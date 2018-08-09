/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _GENFSK_INTERFACE_H_
#define _GENFSK_INTERFACE_H_

#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"
#include "fsl_xcvr.h"
#include "FunctionLib.h"


/*!
 * @addtogroup genfsk
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief GENFSK Link Layer driver version 0.0.1. */
#define GENFSK_DRIVER_VERSION (MAKE_VERSION(0, 0, 1))
/* @} */

/*! @brief GENFSK Protocol Engine interrupt. */
#ifndef gGENFSK_IrqNo_d
#define gGENFSK_IrqNo_d        (Radio_1_IRQn)
#endif

/*! @brief GENFSK Protocol Engine interrupt priority. */
#ifndef gGENFSK_IrqPriority_c
#define gGENFSK_IrqPriority_c  (0x40)
#endif

/*! @brief GENFSK LL Task stack size. */
#ifndef gGENFSK_TaskStackSize_c
#define gGENFSK_TaskStackSize_c (900)
#endif

/*! @brief GENFSK LL Task priority. */
#ifndef gGENFSK_TaskPriority_c
#define gGENFSK_TaskPriority_c (1)
#endif

/*! @brief GENFSK LL total number of available instances. */
#ifndef gGENFSK_InstancesCnt_c
#define gGENFSK_InstancesCnt_c (1)
#endif

/*! @brief GENFSK LL invalid instance ID. */
#define gGENFSK_InvalidIdx_c  (0xFF)

/*!
 * @brief GENFSK Data Rate selections.
 */
typedef enum _genfskDataRate
{
  gGenfskDR1Mbps = 0U,  /*!< GENFSK 1 MBit datarate */
  gGenfskDR500Kbps = 1U,  /*!< GENFSK 500 KBit datarate */
  gGenfskDR250Kbps = 2U  /*!< GENFSK 250 KBit datarate */
} genfskDataRate_t;

/*!
 * @brief GENFSK Radio Mode selections.
 */
typedef enum _genfskRadioMode
{
    /* BT=0.5, h=** */
    gGenfskGfskBt0p5h0p5  = 0U,  /*!< BT=0.5, h=0.5 [BLE at 1MBPS data rate; CS4 at 250KBPS data rate] */
    gGenfskGfskBt0p5h0p32 = 1U,  /*!< BT=0.5, h=0.32*/
    gGenfskGfskBt0p5h0p7  = 2U,  /*!< BT=0.5, h=0.7 [CS1 at 500KBPS data rate] */
    gGenfskGfskBt0p5h1p0  = 3U,  /*!< BT=0.5, h=1.0 [CS4 at 250KBPS data rate] */
    /* BT=** h=0.5 */
    gGenfskGfskBt0p3h0p5 = 4U,  /*!< BT=0.3, h=0.5 [CS2 at 1MBPS data rate] */
    gGenfskGfskBt0p7h0p5 = 5U,  /*!< BT=0.7, h=0.5 */
    gGenfskFsk       = 6U,  /*!< FSK */
    gGenfskMsk       = 7U  /*!< MSK */
} genfskRadioMode_t;

/*! @brief Error codes for the GENFSK driver. */
typedef enum _genfskStatus
{
    gGenfskSuccess_c = 0U,  /*!< Execution successful. */
    gGenfskInvalidParameters_c = 1U,  /*!< Invalid parameters. */
    gGenfskFail_c = 2U,  /*!< Execution failure. */    
    gGenfskNotInitialized_c = 3U, /*!< The GENFSK module was not initialized. */
    gGenfskAlreadyInit_c = 4U,  /*!< Issued by GENFSK_AllocInstance() if the GENFSK module is already initialized. */
    gGenfskBusyRx_c = 5U,  /*!< Transceiver has an active RX sequence. */
    gGenfskBusyTx_c = 6U,  /*!< Transceiver has an active TX sequence. */
    gGenfskBusyPendingRx_c = 7U,  /*!< Transceiver has a pending RX sequence. */
    gGenfskBusyPendingTx_c = 8U,  /*!< Transceiver has a pending TX sequence. */
    gGenfskInstantPassed_c = 9U,  /*!< Issued by GENFSK_StartRx()/GENFSK_StartTx() when an event is programmed too close or in the past. */
    gGenfskAllocInstanceFailed = 10U  /*!< Issued by GENFSK_AllocInstance() when the instance allocation failed. */
} genfskStatus_t;

/*! @brief GENFSK notification events. */
typedef enum _genfskEvent
{
    gGenfskTxEvent = 0x01U,  /*!< TX sequence has completed with a successful packet transmission. */
    gGenfskRxEvent = 0x02U,  /*!< RX sequence has completed with a successful packet reception. */    
    gGenfskNwkAddressMatch = 0x04U,  /*!< Network address match has occurred. */
    gGenfskWakeEvent = 0x08U,  /*!< The SLEEP_TMR has matched GENERIC_FSK_WAKE and DSM exited. */
    gGenfskAllEvents = gGenfskTxEvent | gGenfskRxEvent | gGenfskNwkAddressMatch | gGenfskWakeEvent  /*!< All events. */
} genfskEvent_t;

/*! @brief GENFSK notification events status. */
typedef enum _genfskEventStatus
{
    gGenfskSuccess = 0U,  /*!< Success status. */
    gGenfskRxAllocLengthFail = 1U,  /*!< Allocated RX buffer length is smaller than the received packet length. */
    gGenfskTimeout = 2U,  /*!< RX sequence timeout. */
    gGenfskSyncLost = 3U,  /*!< RX/TX PLL unlock. */
    gGenfskCRCInvalid = 4U,  /*!< CRC invalid for RX packet. Promiscuous mode only! */
    gGenfskH0Fail = 5U,  /*!< H0 violated status. Promiscuous mode only! */
    gGenfskH1Fail = 6U,  /*!< H1 violated status. Promiscuous mode only! */
    gGenfskLengthFail = 7U  /*!< Length field violated status. Promiscuous mode only! */
} genfskEventStatus_t;

/*! @brief LENGTH_BIT_ORD bit definitions. */
typedef enum _genfskPacketCfgLengthBitOrd
{
    gGenfskLengthBitLsbFirst = 0U,  /*!< Bit order of the LENGTH field of the header LSB first. */
    gGenfskLengthBitMsbFirst = 1U   /*!< Bit order of the LENGTH field of the header MSB first. */
} genfskPacketCfgLengthBitOrd_t;

/*! @brief CRC enable bit definitions. */
typedef enum _genfskCrcComputeMode
{    
    gGenfskCrcDisable = 0U,  /*!< CRC functionality disabled. */
    gGenfskCrcEnable = 1U  /*!< CRC functionality enabled. */
} genfskCrcComputeMode_t;

typedef enum _genfskCrcRecvInvalid
{
    gGenfskCrcSupressInvalid = 0U,  /*!< Supress reception of packets with invalid CRC reception. */
    gGenfskCrcRecvInvalid = 1U  /*!< Receive packets with invalid CRC. */
} genfskCrcRecvInvalid_t;

/*! @brief CRC_REF_IN bit definitions. */
typedef enum _genfskCrcCfgCrcRefIn
{
    gGenfskCrcInputNoRef = 0U,  /*!< Do not manipulate input data stream. */
    gGenfskCrcRefInput = 1U  /*!< Reflect each byte in the input stream bitwise. */
} genfskCrcCfgCrcRefIn_t;

/*! @brief CRC_REF_OUT bit definitions. */
typedef enum _genfskCrcCfgCrcRefOut
{
    gGenfskCrcOutputNoRef = 0U,  /*!< Do not manipulate CRC result. */
    gGenfskCrcRefOutput = 1U  /*!< CRC result is to be reflected bitwise (operated on entire word). */
} genfskCrcCfgCrcRefOut_t;

/*! @brief CRC_BYTE_ORD bit definitions. */
typedef enum _genfskCrcCfgCrcByteOrd
{
    gGenfskCrcLSByteFirst = 0U,  /*!< Byte order of the CRC LS Byte first. */
    gGenfskCrcMSByteFirst = 1U   /*!< Bit order of the CRC  MS Byte first. */
} genfskCrcCfgCrcByteOrd_t;

/*! @brief Whitener enable bit definitions. */
typedef enum _genfskWhitenMode
{   
    gGenfskWhitenDisable = 0U,  /*!< Whitener functionality disabled. */
    gGenfskWhitenEnable = 1U,  /*!< Whitener functionality enabled. */
} genfskWhitenMode_t;

/*! @brief WHITEN_START bit definitions. */
typedef enum _genfskWhitenStart
{
    gWhitenStartNoWhitening = 0U,  /*!< No whitening. */
    gWhitenStartWhiteningAtH0 = 1U,  /*!< Start whitening at start-of-H0. */
    gWhitenStartWhiteningAtH1 = 2U,  /*!< Start whitening at start-of-H1 but only if LENGTH > WHITEN_SZ_THR. */
    gWhitenStartWhiteningAtPayload = 3U  /*!< Start whitening at start-of-payload but only if LENGTH > WHITEN_SZ_THR. */
} genfskWhitenStart_t;

/*! @brief WHITEN_END bit definitions. */
typedef enum _genfskWhitenEnd
{
    gWhitenEndAtEndOfPayload = 0U,  /*!< End whiten at end-of-payload. */
    gWhitenEndAtEndOfCrc = 1U  /*!< End whiten at end-of-CRC. */
} genfskWhitenEnd_t;

/*! @brief WHITEN_B4_CRC bit definitions. */
typedef enum _genfskWhitenB4Crc
{
    gCrcB4Whiten = 0U, /*!< CRC before whiten/de-whiten. */
    gWhitenB4Crc = 1U  /*!< Whiten/de-whiten before CRC. */
} genfskWhitenB4Crc_t;

/*! @brief WHITEN_POLY_TYPE bit definitions. */
typedef enum _genfskWhitenPolyType
{
    gGaloisPolyType = 0U,  /*!< A Galois type LFSR is used with the whiten polynomial. */
    gFibonnaciPolyType = 1U  /*!< A Fibonacci type LFSR is used with the whiten polynomial. */
} genfskWhitenPolyType_t;

/*! 
 * @brief WHITEN_REF_IN bit definitions. 
 *
 * @note The input data stream is reflected bit-wise, per byte. Bit 7 becomes bit 0, bit 6 becomes bit 1, etc. 
 * Will only cause the reflection of the payload data bits as they are used in the whiten calculation
 * and will not cause any change in the output bit order.
 */
typedef enum _genfskWhitenCfgRefIn
{
    gGenfskWhitenInputNoRef = 0U,  /*!< Do not manipulate input data stream. */
    gGenfskWhitenRefInput = 1U  /*!< Reflect each byte in the input stream bitwise. */
} genfskWhitenCfgRefIn_t;

/*! @brief WHITEN_PAYLOAD_REINIT bit definitions. */
typedef enum _genfskWhitenCfgPayloadReinit
{
    gGenfskWhitenNoPayloadReinit = 0U,  /*!< Do not re-initialize whitener LFSR at start-of-payload. */
    gGenfskWhitenPayloadReinit = 1U  /*!< Re-initialize whitener LFSR at start-of-payload. */
} genfskWhitenCfgPayloadReinit_t;

/*! @brief MANCHESTER_EN bit definitions. */
typedef enum _genfskManchesterEn
{
    gGenfskManchesterDisable = 0U,  /*!< Disable Manchester encoding (TX) and decoding (RX). */
    gGenfskManchesterEnable = 1U  /*!< Enable Manchester encoding (TX) and decoding (RX). */
} genfskManchesterEn_t;

/*! @brief MANCHESTER_INV bit definitions. */
typedef enum _genfskManchesterInv
{
    gGenfskManchesterNoInv = 0U,  /*!< Manchester coding as per 802.3. */
    gGenfskManchesterInverted = 1U  /*!< Manchester coding as per 802.3 but with the encoding signal inverted. */
} genfskManchesterInv_t;

/*! @brief MANCHESTER_START bit definitions. */
typedef enum _genfskManchesterStart
{
    gGenfskManchesterStartAtPayload = 0U,  /*!< Start Manchester coding at start-of-payload. */
    gGenfskManchesterStartAtHeader = 1U  /*!< Start Manchester coding at start-of-header. */
} genfskManchesterStart_t;

/*! @brief Data packet type bit definitions. */
typedef enum _genfskPacketType
{
    gGenfskFormattedPacket,  /*!< The packets sent or received are formatted, all HW accelerations are available. */
    gGenfskRawPacket  /*!< The packets sent or received are RAW, all HW acceleration is bypassed (limited to 35bytes of payload). */
} genfskPacketType_t;

/*!
 * @brief GENFSK network address type.
 *
 * Network address.
 *
 * @note The LS bytes of this type are used when network address length is less than 4 bytes. 
 */
typedef uint32_t GENFSK_nwk_addr_t;

/*!
 * @brief GENFSK network address matching settings.
 *
 * Definition of the settings for matching a network address.
 */
typedef struct _GENFSK_nwk_addr_match
{
    uint8_t nwkAddrSizeBytes;  /*!< Sync (network) address length in bytes, allowed range is 0..3 representing 1 to 4 bytes long sync addresses. */
    uint8_t nwkAddrThrBits;  /*!< Sync (network) address matching threshold, number of bits that can mismatch and still be considered a match. */
    GENFSK_nwk_addr_t nwkAddr;  /*!< The network address to be matched. */
} GENFSK_nwk_addr_match_t;

/*!
 * @brief GENFSK timestamp type.
 *
 * 64 bits of timestamp.
 *
 * @note The timestamp based on a 1us timer tick. 
 */
typedef uint64_t GENFSK_timestamp_t;

/*! @brief GENFSK radio configure structure. */
typedef struct _GENFSK_radio_config
{
    genfskRadioMode_t radioMode;  /*!< Radio mode for GENFSK radio. See "genfskRadioMode_t". */
    genfskDataRate_t dataRate;  /*!< Data rate for GENFSK radio. See "genfskDataRate_t". */
} GENFSK_radio_config_t;

/*! @brief GENFSK packet format configure structure. */
typedef struct _GENFSK_packet_config
{
    uint8_t preambleSizeBytes;  /*!< Preamble length in bytes, allowed range is 0..7 representing 1 to 8 bytes long preambles. */    
    genfskPacketType_t packetType;  /*!< Packet type. See "genfskPacketType_t". */
    uint8_t lengthSizeBits;  /*!< Number of bits in the LENGTH field. */
    genfskPacketCfgLengthBitOrd_t lengthBitOrder;  /*!< Bit order for the LENGTH field of the header. See "genfskPacketCfgLengthBitOrd_t". */
    uint8_t syncAddrSizeBytes;  /*!< Sync (network) address length in bytes, allowed range is 0..3 representing 1 to 4 bytes long sync addresses. */
    int8_t lengthAdjBytes;  /*!< Signed adjustment to the length field for TX and RX. A value of 0 (default) means LENGTH is interpreted as PAYLOAD + CRC. */
    uint8_t h0SizeBits;  /*!< Number of bits in the H0 field. */
    uint8_t h1SizeBits;  /*!< Number of bits in the H1 field. */    
    uint16_t h0Match;  /*!< Bits which must match the H0 portion of a received packet for valid packet reception. */
    uint16_t h0Mask;  /*!< Mask to select which bits of H0 must match the h0_match field. */
    uint16_t h1Match;  /*!< Bits which must match the H1 portion of a received packet for valid packet reception. */
    uint16_t h1Mask;  /*!< Mask to select which bits of H1 must match the h1_match field. */
} GENFSK_packet_config_t;

/*! @brief GENFSK CRC module configure structure. */
typedef struct _GENFSK_crc_config
{
    genfskCrcComputeMode_t crcEnable;  /*!< Software override of the HW-computed CRC for TX. See "genfskCrcComputeMode_t". */
    genfskCrcRecvInvalid_t crcRecvInvalid;  /*!< Receive packets with invalid CRC. See "genfskRecvInvalidCrc_t". */
    uint8_t crcSize;  /*!< Number of CRC octets, allowed range is 0..4. */
    uint8_t crcStartByte;  /*!< Start CRC with this byte position. Byte #0 is the first byte of Sync Address. */
    genfskCrcCfgCrcRefIn_t crcRefIn;  /*!< CRC reflect input. See "genfskCrcCfgCrcRefIn_t". */
    genfskCrcCfgCrcRefOut_t crcRefOut;  /*!< CRC reflect output. See "genfskCrcCfgCrcRefOut_t". */
    genfskCrcCfgCrcByteOrd_t crcByteOrder;  /*!< CRC byte order. See "genfskCrcCfgCrcByteOrd_t". */
    uint32_t crcSeed;  /*!< CRC Seed value. Initial value for CRC LFSR. */
    uint32_t crcPoly;  /*!< CRC Polynomial value. */
    uint32_t crcXorOut;  /*!< XOR mask for CRC result (for no mask, should be 0). */
} GENFSK_crc_config_t;

/*!
 * @brief GENFSK Whitener module configure structure.
 *
 * @note Whitening and Manchester encoding are mutually exclusive. 
 */
typedef struct _GENFSK_whitener_config
{
    genfskWhitenMode_t whitenEnable;  /*!<  Enable/Disable HW (de)whitening on RX and TX packets. See "genfskWhitenMode_t". */
    genfskWhitenStart_t whitenStart;  /*!< Configure Whitener start point. See "genfskWhitenStart_t". */
    genfskWhitenEnd_t whitenEnd;  /*!< Configure end-of-whitening. See "genfskWhitenEnd_t". */
    genfskWhitenB4Crc_t whitenB4Crc;  /*!< Configure for whitening-before-CRC. See "genfskWhitenB4Crc_t". */
    genfskWhitenPolyType_t whitenPolyType;  /*!< Whiten polynomial type. See "genfskWhitenPolyType_t". */
    genfskWhitenCfgRefIn_t whitenRefIn;  /*!< Whiten reflect input. See "genfskWhitenCfgRefIn_t". */
    genfskWhitenCfgPayloadReinit_t whitenPayloadReinit;  /*!< Configure for whitener re-initialization. See "genfskWhitenCfgPayloadReinit_t. */
    uint8_t whitenSize;  /*!< Length of whitener LFSR. Maximum value 9. */
    uint16_t whitenInit;  /*!< Initialization value for Whitening/De-whitening. Maximum 9 bits. */
    uint16_t whitenPoly;  /*!< Whitener polynomial. The polynomial value must be right-justified if smaller than 9-bits. Maximum 9 bits. */
    uint16_t whitenSizeThr;  /*!< Whitener size threshold. Maximum packet length required to enable whiten. Requires WHITEN_START 2 or 3. */
    genfskManchesterEn_t manchesterEn;  /*!< Configure for Manchester Encoding/Decoding. See "genfskManchesterEn_t". */
    genfskManchesterStart_t manchesterStart;  /*!< Configure for inverted Manchester Encoding. See "genfskManchesterStart_t". */
    genfskManchesterInv_t manchesterInv;  /*!< Configure Manchester Encoding start point. See "genfskManchesterInv_t". */
} GENFSK_whitener_config_t;

/*!
 * @brief GENFSK bitstream processing configuration.
 *
 * Configuration of the bitstream processing to be done for GENFSK.
 */
typedef struct _GENFSK_bitproc
{
    GENFSK_crc_config_t *crcConfig;  /*!< CRC module configuration structure. */
    GENFSK_whitener_config_t *whitenerConfig;  /*!< Whitener module configuration structure. */
} GENFSK_bitproc_t;

/*! @brief Data structure for GENFSK packet header. */
typedef struct _GENFSK_packet_header_t
{
    uint16_t h0Field;  /*!< H0 field value. */
    uint16_t lengthField;  /*!< LENGTH field value. */
    uint16_t h1Field;  /*!< H1 field value. */
} GENFSK_packet_header_t;

/*! @brief Data structure for GENFSK packet. */
typedef struct _GENFSK_packet_t
{
    GENFSK_nwk_addr_t addr;  /*!< Network address. */
    GENFSK_packet_header_t header;  /*!< Packet header data structure. */
    uint8_t *payload;  /*!< Payload (+ CRC if reception) buffer. */
} GENFSK_packet_t;

/*!
 * @brief Packet Received callback function pointer type.
 *
 * @param pBuffer The pointer to the buffer used for reception.
 * @param packet_length The allocated pkt_buffer size for the maximum packet length that can be received.
 * @param timestamp The timestamp for the received packet in microseconds.
 * @param rssi The RSSI for the received packet. 
 * @param crcValid If set, the CRC for the received packet is valid. Else CRC is invalid. 
 */
typedef void ( *genfskPacketReceivedCallBack_t ) (uint8_t *pBuffer, uint16_t bufferLength, uint64_t timestamp, uint8_t rssi, uint8_t crcValid);

/*!
 * @brief Event notification callback function pointer type.
 *
 * @param event Reason the callback is being invoked. See "genfskEvent_t". 
 * @param eventStatus The status of the event. See "genfskEventStatus_t".
 */
typedef void ( *genfskEventNotifyCallBack_t ) (genfskEvent_t event, genfskEventStatus_t eventStatus);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
    
/*!
 * @name GENFSK functional Operation
 * @{
 */

/*!
 * @brief Initializes the GENFSK LL.
 *
 * This function initializes the GENFSK LL. 
 *    
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_Init(void);
    
/*!
 * @brief Allocates a GENFSK LL instance.
 *
 * This function allocates the GENFSK LL module and initilizes the instance according to the protocol and processing chain settings.
 * 
 * 
 * @param pInstanceId The pointer which will save the allocated instance. gGENFSK_InvalidIdx_c if the allocation failed.
 * @param radioConfig The radio configuration for which the GENFSK LL should be configured.
 * @param packetConfig The packet configuration for which the GENFSK LL should be configured.
 * @param bitProcConfig The bitstream processing for which the GENFSK LL should be configured.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 *
 * @warning Should be called after GENFSK_Init();
 */
genfskStatus_t GENFSK_AllocInstance(uint8_t *pInstanceId, GENFSK_radio_config_t *radioConfig, GENFSK_packet_config_t *packetConfig, GENFSK_bitproc_t *bitProcConfig);

/*!
 * @brief Sets the radio configuration for the current GENFSK LL instance.
 *
 * This function initialize the radio and sets the radio configuration.
 *
 * @param instanceId The ID of the instance.
 * @param radioConfig The radio configuration to be set for GENFSK LL.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_RadioConfig(uint8_t instanceId, GENFSK_radio_config_t *radioConfig);

/*!
 * @brief Sets the packet configuration for the current GENFSK LL instance.
 *
 * This function sets the packet configuration.
 *
 * @param instanceId The ID of the instance.
 * @param packetConfig The packet configuration to be set in GENFSK LL.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_SetPacketConfig(uint8_t instanceId, GENFSK_packet_config_t *packetConfig);

/*!
 * @brief Returns the packet configuration currently set in GENFSK LL.
 *
 * This function returns the packet configuration currently set in GENFSK LL.
 *
 * @param instanceId The ID of the instance.
 * @param packetConfig The stored packet configuration.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GetPacketConfig(uint8_t instanceId, GENFSK_packet_config_t *packetConfig);

/*!
 * @brief Sets the CRC configuration for the current GENFSK LL instance.
 *
 * This function sets the CRC configuration.
 *
 * @param instanceId The ID of the instance.
 * @param crcConfig The CRC configuration to be set in GENFSK LL.
 * 
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_SetCrcConfig(uint8_t instanceId, GENFSK_crc_config_t *crcConfig);

/*!
 * @brief Returns the CRC configuration currently set in GENFSK LL.
 *
 * This function returns the CRC configuration currently set in GENFSK LL.
 *
 * @param instanceId The ID of the instance.
 * @param crcConfig The stored CRC configuration.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GetCrcConfig(uint8_t instanceId, GENFSK_crc_config_t *crcConfig);

/*!
 * @brief Sets whitening configuration for the current GENFSK LL instance.
 *
 * This function sets the whitening configuration.
 *
 * @param instanceId The ID of the instance.
 * @param whitenerConfig The whitening configuration to be set in GENFSK LL.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_SetWhitenerConfig(uint8_t instanceId, GENFSK_whitener_config_t *whitenerConfig);

/*!
 * @brief Returns the whitening configuration currently set in GENFSK LL.
 *
 * This function returns the whitening configuration currently set in GENFSK LL.
 *
 * @param instanceId The ID of the instance.
 * @param whitenerConfig The stored whitening configuration.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GetWhitenerConfig(uint8_t instanceId, GENFSK_whitener_config_t *whitenerConfig);

/*!
 * @brief De-initializes the GENFSK LL instance.
 *
 * This function sets all GENFSK registers values to reset values and disables GENFSK LL interrupt if no other
 * instance is initialized.
 *
 * @param instanceId The ID of the instance.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_FreeInstance(uint8_t instanceId);

/*!
 * @brief Reset the GENFSK LL to default values.
 *
 * This function reset the GENFSK LL registers values to the reset values.
 *
 * @param instanceId The ID of the instance.
 */
void GENFSK_ResetToDefaults(uint8_t instanceId);

/*!
 * @brief Controls setting one of the network address match locations.
 *
 * This function set the network address matching.
 *
 * @param instanceId The ID of the instance.
 * @param location the location number to set, valid range is 0..3. This location will be enabled if there are no errors during the setting process.
 * @param nwkAddressSettings the settings to be applied.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */ 
genfskStatus_t GENFSK_SetNetworkAddress(uint8_t instanceId, uint8_t location, GENFSK_nwk_addr_match_t *nwkAddressSettings);

/*!
 * @brief Sets the event mask for genfskEventNotifyCallBack_t callback.
 *
 * Sets the event mask for genfskEventNotifyCallBack_t.
 *
 * @param instanceId The ID of the instance.
 * @param mask The event mask specifies which notification events are sent by genfskEventNotifyCallBack_t. See "genfskEvent_t".
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */ 
genfskStatus_t GENFSK_SetEventMask(uint8_t instanceId, uint32_t mask);

/*!
 * @brief Returns the event mask for genfskEventNotifyCallBack_t callback.
 *
 * @param instanceId The ID of the instance.
 * Returns the current enabled events for genfskEventNotifyCallBack_t.
 *
 * @retval genfskEvent_t.
 */ 
uint32_t GENFSK_GetEventMask(uint8_t instanceId);

/*!
 * @brief Returns the network address set at location.
 *
 * This function enables setting the network address matching.
 *
 * @param instanceId The ID of the instance.
 * @param location the location number to set, valid range is 0..3. This location will be enabled if there are no errors during the setting process.
 * @param nwkAddressSettings the stored network address settings at the specified location.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */ 
genfskStatus_t GENFSK_GetNetworkAddress(uint8_t instanceId, uint8_t location, GENFSK_nwk_addr_match_t *nwkAddressSettings);

/*!
 * @brief Controls enabling one of the network address match locations.
 *
 * This function enables one of the network address matching.
 *
 * @param instanceId The ID of the instance.
 * @param location the location number to disable, valid range is 0..3. This location will be enabled if there are no errors during the setting process.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */ 
genfskStatus_t GENFSK_EnableNetworkAddress(uint8_t instanceId, uint8_t location);

/*!
 * @brief Controls disabling one of the network address match locations.
 *
 * This function disables one of the network address matching.
 *
 * @param instanceId The ID of the instance.
 * @param location the location number to disable, valid range is 0..3. This location will be disabled if there are no errors during the setting process.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */ 
genfskStatus_t GENFSK_DisableNetworkAddress(uint8_t instanceId, uint8_t location);

/*!
 * @brief Sets the channel number.
 *
 * This function sets the channel number.
 *
 * @param instanceId The ID of the instance.
 * @param channelNum The channel number on which to transmit and receive, 0 <= channelNum <= 127; Formula: F = (2360 + channelNum) [in MHz].
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_SetChannelNumber(uint8_t instanceId, uint8_t channelNum);

/*!
 * @brief Returns the channel number currently set in GENFSK LL.
 *
 * This function returns the channel number currently set in GENFSK LL.
 *
 * @param instanceId The ID of the instance.
 *
 * @retval The channel number currently set, 0 <= channelNum <= 127; Formula: F = (2360 + channelNum) [in MHz].
 */
uint8_t GENFSK_GetChannelNumber(uint8_t instanceId);

/*!
 * @brief Sets the power level for transmission.
 *
 * This function sets power level for transmission.
 *
 * @param instanceId The ID of the instance.
 * @param txPowerLevel The power level for transmission, 0 <= txPowerLevel <= 32.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_SetTxPowerLevel(uint8_t instanceId, uint8_t txPowerLevel);

/*!
 * @brief Returns the power level currently set in GENFSK LL.
 *
 * @param instanceId The ID of the instance.
 * This function returns the power level currently set in GENFSK LL.
 *
 * @retval The power level for transmission, 0 <= txPowerLevel <= 32.
 */
uint8_t GENFSK_GetTxPowerLevel(uint8_t instanceId);

/*!
 * @brief Performs a transmission.
 *
 * This function performs a transmission of GENFSK LL Packet.
 *
 * @param instanceId The ID of the instance.
 * @param pBuffer The pointer to a buffer containing the packet body compliant to the previously configured settings.
 * @param bufLengthBytes The buffer length in bytes.
 * @param txStartTime The time at which to start transmission. Set 0 for immediate transmission.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 *
 * @warning Timebase roll over at 24 bits (~16.7 seconds) must be considered in setting the txStartTime.
 */
genfskStatus_t GENFSK_StartTx(uint8_t instanceId, uint8_t *pBuffer, uint16_t bufLengthBytes, GENFSK_timestamp_t txStartTime);

/*!
 * @brief Cancels pending TX events.
 *
 * This function cancels pending TX events for the current active instance but do not abort a TX-in-progress.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_CancelPendingTx(void);

/*!
 * @brief Performs a receive operation.
 *
 * This function performs a receive operation.
 *
 * @param instanceId The ID of the instance.
 * @param pBuffer The pointer to a buffer used for reception.
 * @param maxBufLengthBytes The allocated pBuffer size for the maximum packet length that can be received.
 * @param rxStartTime The time at which to start receive. Set 0 for immediate receive.
 * @param rxDuration The duration of the receive operation. Set 0 for continuous reception.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 *
 * @warning Timebase roll over at 24 bits (~16.7 seconds) must be considered in setting the rxStartTime.
 */
genfskStatus_t GENFSK_StartRx(uint8_t instanceId, uint8_t *pBuffer, uint16_t maxBufLengthBytes, GENFSK_timestamp_t rxStartTime, GENFSK_timestamp_t rxDuration);

/*!
 * @brief Cancels pending RX events.
 *
 * This function cancels pending RX events for the current active instance but do not abort a RX-in-progress.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_CancelPendingRx(void);

/*!
 * @brief Cancels all pending events.
 *
 * This function cancels all pending events for the current active instance and abort any sequence-in-progress.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_AbortAll(void);

/*!
 * @brief Fetches the current value of the GENFSK LL timebase.
 *
 * This function fetches the current value of the timebase for the LL.
 *
 * @retval The value of the timebase, in microseconds.
 *
 * @warning Any use of the timestamp value must allow for processing delays.
 */
GENFSK_timestamp_t GENFSK_GetTimestamp(void);

/*!
 * @brief Converts a packet buffer to a byte array format to be sent by GENFSK LL.
 *
 * This function is used before GENFSK_StartTx() in order to convert the formatted packet in a byte array to be sent over the air.
 * The byte array will have the format :
 *      NWK_ADDRESS | H0 | LENGTH | H1 | PAYLOAD | CRC 
 *
 * @param instanceId The ID of the instance for which the packet to be formatted.
 * @param pPacket Pointer to the packet structure to be formatted.
 * @param pBuffer Pointer to the byte array formatted buffer.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_PacketToByteArray(uint8_t instanceId, GENFSK_packet_t *pPacket, uint8_t *pBuffer);

/*!
 * @brief Converts a received byte array formatted packet in GENFSK_packet_t format.
 *
 * This function is used after a packet is received in order to convert the byte array received over the air in GENFSK_packet_t format.
 *
 * @param instanceId The ID of the instance for which the packet to be formatted.
 * @param pBuffer Pointer to the byte array formatted buffer.
 * @param pPacket Pointer to the packet structure to store the formatted packet.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_ByteArrayToPacket(uint8_t instanceId, uint8_t *pBuffer, GENFSK_packet_t *pPacket);

/*!
 * @brief Registers the callback functions packet received and event notifications.
 *
 * This function register the callback functions for packet received and event notifications.
 *
 * @param instanceId The ID of the instance.
 * @param packetReceivedCallback Packet received callback.
 * @param eventCallback Event callback.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_RegisterCallbacks(uint8_t instanceId, genfskPacketReceivedCallBack_t packetReceivedCallback, genfskEventNotifyCallBack_t eventCallback);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _GENFSK_INTERFACE_H_ */
