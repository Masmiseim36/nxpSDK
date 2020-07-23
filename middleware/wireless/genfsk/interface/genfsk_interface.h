/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#ifndef _GENFSK_INTERFACE_H_
#define _GENFSK_INTERFACE_H_

#include <stdbool.h>
#include "genfsk_sw_version.h"
#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"
#if defined(KW37Z4_SERIES) || defined(KW37A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW38A4_SERIES)
#include "nxp2p4_xcvr.h"
#else
#include "fsl_xcvr.h"
#endif
#include "FunctionLib.h"


/*!
 * @addtogroup genfsk
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(KW37Z4_SERIES) || defined(KW37A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW38A4_SERIES)
#ifndef RADIO_IS_GEN_3P5
#define RADIO_IS_GEN_3P5 1
#endif /* RADIO_IS_GEN_3P5 */
#define RADIO_IS_GEN_3P0 1
#endif

#ifndef gGenfskPreserveXcvrDacTrimValue_d
#define gGenfskPreserveXcvrDacTrimValue_d 0
#endif

#ifndef gGenfskXcvrInitRetryCount_c
#define gGenfskXcvrInitRetryCount_c (10U)
#endif

/*! @brief Generic FSK build version  */
#define gGenFskVerMajor_c   3
#define gGenFskVerMinor_c   0
#define gGenFskVerPatch_c   1
#define gGenFskBuildNo_c    0

#define gGenFskModuleId_c  0xD0

#define gGenFskVerString_c "Generic FSK v" \
                        QUH(gGenFskVerMajor_c) "." \
                        QUH(gGenFskVerMinor_c) "." \
                        QUH(gGenFskVerPatch_c) "." \
                        QUH(gGenFskBuildNo_c)

/*! @name Driver version */
/*@{*/
/*! @brief GENFSK Link Layer driver version 0.0.1. */
#define GENFSK_DRIVER_VERSION (MAKE_VERSION(0, 0, 1))
/* @} */

/*! @brief GENFSK Protocol Engine interrupt. */
#ifndef gGENFSK_IrqNo_d
#if (defined(CPU_MKW21Z256VHT4) || defined(CPU_MKW21Z512VHT4) || defined(CPU_MKW31Z256CAx4) || \
     defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAx4) || defined(CPU_MKW31Z512VHT4) || \
     defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512VHT4) || \
     defined(CPU_MKW35A512VFP4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW36A512VFP4) || \
     defined(CPU_MKW36A512VHT4) || defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4) || \
     defined(KW37Z4_SERIES)     || defined(KW37A4_SERIES)     || \
     defined(KW38Z4_SERIES)     || defined(KW38A4_SERIES))
#define gGENFSK_IrqNo_d        (Radio_1_IRQn)
#elif (defined(CPU_K32W042S1M2VPJ_cm0plus))
#define gGENFSK_IrqNo_d        (RF0_1_IRQn)
#endif
#endif

/*! @brief GENFSK Protocol Engine interrupt priority. */
#ifndef gGENFSK_IrqPriority_c
#define gGENFSK_IrqPriority_c  (0x40U)
#endif

/*! @brief GENFSK LL Task stack size. */
#ifndef gGENFSK_TaskStackSize_c
#define gGENFSK_TaskStackSize_c (900U)
#endif

/*! @brief GENFSK LL Task priority. */
#ifndef gGENFSK_TaskPriority_c
#define gGENFSK_TaskPriority_c (1U)
#endif

/*! @brief GENFSK LL total number of available instances. */
#ifndef gGENFSK_InstancesCnt_c
#define gGENFSK_InstancesCnt_c (1U)
#endif

/*! @brief GENFSK does not usw MWS */
#define gGENFSK_NoMwsControl_c  (0)

/*! @brief The GENFSK Link Layer controls the MWS */
#define gGENFSK_LLMwsControl_c  (1)

/*! @brief The Application Layer controls the MWS */
#define gGENFSK_AppMwsControl_c (2)

/*! @brief GENFSK MWS control option. */
#ifndef gGENFSK_MwsControl_c
#define gGENFSK_MwsControl_c     (gGENFSK_NoMwsControl_c)
#endif

/*! @brief GENFSK API parameters check. */
#ifndef gGENFSK_CheckParams_c
#define gGENFSK_CheckParams_c (1U)
#endif

/*! @brief GENFSK LL invalid instance ID. */
#define gGENFSK_InvalidIdx_c  (0xFFU)

/*! @brief GENFSK Timer invalid ID. */
#define gGENFSK_InvalidTimerId_c (0xFFU)

/*! @brief GENFSK events. */
#define gGenfskTxEvent (0x01UL)  /*!< TX sequence has completed with a successful packet transmission. */
#define gGenfskRxEvent (0x02UL)  /*!< RX sequence has completed with a successful packet reception. */
#define gGenfskNwkAddressMatch (0x04UL) /*!< Network address match has occurred. */
#define gGenfskWakeEvent (0x08UL)  /*!< The SLEEP_TMR has matched GENERIC_FSK_WAKE and DSM exited. */
#define gGenfskDefaultEnabledEvents (gGenfskTxEvent | gGenfskRxEvent | gGenfskWakeEvent)     /*!< default enabled events. */
#define gGenfskAllEvents (gGenfskTxEvent | gGenfskRxEvent | gGenfskNwkAddressMatch | gGenfskWakeEvent)  /*!< All events. */
#define gGenfskPromiscuousEvent (0x80000000UL) /*!< Promiscuous event . */

#if defined(GCOV_DO_COVERAGE)
#define GENFSK_STATIC
#define GENFSK_STATIC_INLINE
#else
#define GENFSK_STATIC        static
#define GENFSK_STATIC_INLINE static inline
#endif

/*!
 * @brief GENFSK Data Rate selections.
 */
typedef enum _genfskDataRate
{
    gGenfskDR1Mbps = 0U,    /*!< GENFSK 1 MBit datarate */
    gGenfskDR500Kbps = 1U,  /*!< GENFSK 500 KBit datarate */
    gGenfskDR250Kbps = 2U,  /*!< GENFSK 250 KBit datarate */
#if RADIO_IS_GEN_3P0
    gGenfskDR2Mbps = 3U,     /*!< GENFSK 2 MBit datarate */
#endif /* RADIO_IS_GEN_3P0 */
    gGenfskDRInvalid = 4U   /*!< Invalid */
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
#ifndef RADIO_IS_GEN_3P5
    /* BT=** h=0.5 */
    gGenfskGfskBt0p3h0p5 = 4U,  /*!< BT=0.3, h=0.5 [CS2 at 1MBPS data rate] */
    gGenfskGfskBt0p7h0p5 = 5U,  /*!< BT=0.7, h=0.5 */
    gGenfskFsk       = 6U,  /*!< FSK */
#endif /* RADIO_IS_GEN_3P5 */
    gGenfskMsk       = 7U,  /*!< MSK */
    gGenfskRadioModeInvalid = 8U  /*!< Invalid */
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
typedef uint32_t genfskEvent_t;

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
    gGenfskRawPacket  /*!< The packets sent or received are RAW, all HW acceleration is bypassed (limited to 35 bytes of payload for RADIO_IS_GEN_3P0 and to 1027 bytes of payload for RADIO_IS_GEN_3P5). */
} genfskPacketType_t;

/*! @brief Link layer Mode definition. */
typedef enum _genfsMode
{
    gGenfskGllMode = 0U, /*!< 0000b - GLL Mode */
    gGenfskPanMode = 1U, /*!< 0001b - PAN Mode */
    gGenfskFanMode = 2U, /*!< 0010b - FAN Mode */
    gGenfskHybridDualPanMode = 3U, /*!< 0011b - Hybrid Dual PAN Mode */
    gGenfskWmbusAMode = 4U, /*!< 0100b - WMBUSA Mode */
    gGenfskWmbusBMode = 5U, /*!< 0101b - WMBUSB Mode */
    gGenfskAntMode = 6U,    /*!< 0110b - ANT Mode */
    gGenfskLeg0LiMode = 7U, /*!< 0111b - LEG0 Mode, packet has Length-field */
    gGenfskLeg0noLiMode = 8U, /*!< 1000b - LEG0 Mode, packet has no Length-field */
    gGenfskBleUncodedMode = 9U, /*!< 1001b - BLE Uncoded Mode */
    gGenfskBleLrMode = 10U, /*!< 1010b - BLE LR Mode */
    gGenfskBleConcurentMode = 11U, /*!< 1011b - BLE Concurrent Mode (RX configuration only; TX uses either BLE_UNCODED or BLE_LR configuration) */
    gGenfskGtmMode = 15U, /*!< 1111b - GTM Mode */
} genfskMode_t;

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

/*! @brief GENFSK timer ID type. */
typedef uint8_t genfskTimerId_t;

/*! @brief GENFSK timeout callback. */
typedef void (*genfskTimeCallback_t) (void);

/*! @brief GENFSK time structure. */
typedef struct _GENFSK_TimeEvent
{
    GENFSK_timestamp_t    timestamp;
    genfskTimeCallback_t  callback;
} GENFSK_TimeEvent_t;

/*! @brief GENFSK radio configure structure. */
typedef struct _GENFSK_radio_config
{
    genfskRadioMode_t radioMode;  /*!< Radio mode for GENFSK radio. See "genfskRadioMode_t". */
    genfskDataRate_t dataRate;  /*!< Data rate for GENFSK radio. See "genfskDataRate_t". */
} GENFSK_radio_config_t;

/*! @brief GENFSK mode configure structure. */
typedef struct _GENFSK_mode_config
{
    genfskMode_t mode;  /*!< Mode for GENFSK link layer. See "genfskMode_t". */
    uint8_t stayInRx;  /*!< Enable the ability to recycle and stay in receive even after an RX_IRQ, to avoid the full RX re-warmup. */
    uint32_t turnaroundTime; /*!< Tx to Rx or Rx to Tx delay in us. Only applies to combined RXx/Tx sequences. Not used otherwise */
} GENFSK_mode_config_t;

/*! @brief GENFSK packet format configure structure. */
typedef struct _GENFSK_packet_config
{
    uint16_t preambleSizeBytes;  /*!< Preamble length in bytes, allowed range is 0..511 representing 1 to 512 bytes long preambles. */    
    genfskPacketType_t packetType;  /*!< Packet type. See "genfskPacketType_t". */
    uint8_t lengthSizeBits;  /*!< Number of bits in the LENGTH field. */
    genfskPacketCfgLengthBitOrd_t lengthBitOrder;  /*!< Bit order for the LENGTH field of the header. See "genfskPacketCfgLengthBitOrd_t". */
    uint8_t syncAddrSizeBytes;  /*!< Sync (network) address length in bytes, allowed range is 0..3 representing 1 to 4 bytes long sync addresses. */
    int16_t lengthAdjBytes;  /*!< Signed adjustment to the length field for TX and RX. A value of 0 (default) means LENGTH is interpreted as PAYLOAD + CRC. */
    uint8_t h0SizeBits;  /*!< Number of bits in the H0 field. */
    uint8_t h1SizeBits;  /*!< Number of bits in the H1 field. */    
    uint16_t h0Match;  /*!< Bits which must match the H0 portion of a received packet for valid packet reception. */
    uint16_t h0Mask;  /*!< Mask to select which bits of H0 must match the h0_match field. */
    uint16_t h1Match;  /*!< Bits which must match the H1 portion of a received packet for valid packet reception. */
    uint16_t h1Mask;  /*!< Mask to select which bits of H1 must match the h1_match field. */
#if defined (RADIO_IS_GEN_3P5)
    uint8_t  preambleBytePattern; /*if value is not 0, the preamble pattern will be repeated preambleSizeBytes+1*/
#endif    
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
typedef void ( *genfskPacketReceivedCallBack_t ) (uint8_t *pBuffer, uint16_t bufferLength, uint64_t timestamp, int8_t rssi, uint8_t crcValid);

/*!
 * @brief Event notification callback function pointer type.
 *
 * @param event Reason the callback is being invoked. See "genfskEvent_t". 
 * @param eventStatus The status of the event. See "genfskEventStatus_t".
 */
typedef void ( *genfskEventNotifyCallBack_t ) (genfskEvent_t event, genfskEventStatus_t eventStatus);

/*! @brief GTM packet payload type definition. */
typedef enum 
{
    genfskGtmPrbs9Payload = 0U,         /*!<  PRBS9 pattern, same as BTLE. */
    genfskGtm8BitPatternPayload = 1U,   /*!<  8 bit pattern from GTM_PDU[7:0]. */
    genfskGtmPrbs13Payload = 2U,        /*!<  PRBS13 pattern. */
    genfskGtmPrbs15Payload = 3U,        /*!<  PRBS15 pattern. */
    genfskGtm32BitPatternPayload = 4U,  /*!<  32 bit pattern from GTM_PDU[31:0]. */
    genfskGtmPktRamDataPayload = 5U,    /*!<  Data taken from packet RAM. */
    genfskGtmPktPayloadMax = 6U,        /*!<  Error. */
} genfskGtmPktPayload_t;

/*! @brief Data structure for GENFSK GTM TX packet configuration. */
typedef struct
{
    uint16_t packet_count;                  /*!<  Desired count for GTM TX packets. */
    uint32_t pdu_length;                    /*!<  length of the test packet payload*/
    genfskGtmPktPayload_t pdu_type;   /*!<  Type of PDU to send in GTM TX. */
    uint32_t pattern;                       /*!<  8 or 32 bit PDU pattern to use for GTM TX pattern modes. */
    uint32_t gtm_ipd;                       /*!<  IPD time (microseconds) inter packet transmision time */
    uint32_t warmup_time_delay;             /*!<  delay time between 1st Tx warmup time and GTM_IN_TX asserted*/
} GENFSK_gtm_tx_config_t;

/*! @brief Data structure for GENFSK GTM RX packet configuration. */
typedef struct
{
    bool_t ipd_check_dis;                     /*!<  Set to 1 to disable IPD check in GTM RX. */
    bool_t pkt_count_check_dis;               /*!<  Set to 1 to disable packet count check in GTM RX. */
    uint16_t packet_count;                  /*!<  Expected number of packets to be received. */
    uint32_t pdu_length;                    /*!<  Expected packet length to calculate the longest packet duration. */
    uint32_t recycle_time;                  /*!<  Time between RX warmdown and RX warmup */
    uint32_t gtm_ipd;                       /*!<  IPD time (microseconds) inter packet transmision time */
} GENFSK_gtm_rx_config_t;

/*! @brief Data structure for GENFSK GTM received packet statistics. */
typedef struct
{
    uint16_t packet_count;                  /*!<  When RX, count the number of IPD slots after first AA, When TX, count the number of packets sent. */
    uint16_t good_packet_count;             /*!<  GTM Bad Packet Counter (CRC fail) */
    uint16_t bad_packet_count;              /*!<  GTM Good Packet Counter (CRC pass) */
    bool_t   is_activity_enabled;           /*!<  TRUE, if GTM RX or TX activity not yet completed. This is the value of GTM_IN_RX bit or 
                                                  GTM_IN_TX bit from GTM_CTRL register. */
} GENFSK_gtm_packet_count_t;

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

 * @retval gGenfskSuccess_c if success or the failure reason.
 *
 * @warning Should be called after GENFSK_Init();
 */
genfskStatus_t GENFSK_AllocInstance(uint8_t *pInstanceId, const GENFSK_radio_config_t *radioConfig, const GENFSK_packet_config_t *packetConfig, const GENFSK_bitproc_t *bitProcConfig);

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
genfskStatus_t GENFSK_RadioConfig(uint8_t instanceId, const GENFSK_radio_config_t *radioConfig);

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
genfskStatus_t GENFSK_SetPacketConfig(uint8_t instanceId, const GENFSK_packet_config_t *packetConfig);

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
 * @brief Sets the mode configuration for the current GENFSK LL instance.
 *
 * This function sets the mode configuration.
 *
 * @param instanceId The ID of the instance.
 * @param modeConfig The mode configuration to be set in GENFSK LL.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_SetModeConfig(uint8_t instanceId, const GENFSK_mode_config_t *modeConfig);

/*!
 * @brief Returns the mode configuration currently set in GENFSK LL.
 *
 * This function returns the mode configuration currently set in GENFSK LL.
 *
 * @param instanceId The ID of the instance.
 * @param modeConfig The stored mode configuration.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GetModeConfig(uint8_t instanceId, GENFSK_mode_config_t *modeConfig);

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
genfskStatus_t GENFSK_SetCrcConfig(uint8_t instanceId, const GENFSK_crc_config_t *crcConfig);

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
genfskStatus_t GENFSK_SetWhitenerConfig(uint8_t instanceId, const GENFSK_whitener_config_t *whitenerConfig);

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
genfskStatus_t GENFSK_SetNetworkAddress(uint8_t instanceId, uint8_t location, const GENFSK_nwk_addr_match_t *nwkAddressSettings);

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
genfskStatus_t GENFSK_SetEventMask(uint8_t instanceId, uint32_t eventMask);

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
genfskStatus_t GENFSK_StartTx(uint8_t instanceId, const uint8_t *pBuffer, uint16_t bufLengthBytes, GENFSK_timestamp_t txStartTime);

/*!
 * @brief Performs a transmission followed by a receive operation.
 *
 * This function performs a transmission of GENFSK LL Packet.
 *
 * @param instanceId The ID of the instance.
 * @param pBuffer The pointer to a buffer containing the packet body compliant to the previously configured settings.
 * @param bufLengthBytes The buffer length in bytes.
 * @param txStartTime The time at which to start transmission. Set 0 for immediate transmission.
 * @param followedByRx If set to 1, RX sequence will automatically start after Tx completion.
 * @param rxDuration The duration of the receive operation. Set 0 for continuous reception. Only valid if followedByRx is set to 1
 * @param pRxBuffer The pointer to a buffer used for reception.
 * @param maxBufLengthBytes The allocated pBuffer size for the maximum packet length that can be received.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 *
 * @warning Timebase roll over at 24 bits (~16.7 seconds) must be considered in setting the txStartTime.
 */
genfskStatus_t GENFSK_StartTxRx(uint8_t instanceId, const uint8_t *pBuffer, uint16_t bufLengthBytes, GENFSK_timestamp_t txStartTime, bool followedByRx, GENFSK_timestamp_t rxDuration, uint8_t *pRxBuffer, uint16_t maxBufLengthBytes);

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
 * @param pRxBuffer The pointer to a buffer used for reception.
 * @param maxBufLengthBytes The allocated pBuffer size for the maximum packet length that can be received.
 * @param rxStartTime The time at which to start receive. Set 0 for immediate receive.
 * @param rxDuration The duration of the receive operation. Set 0 for continuous reception.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 *
 * @warning Timebase roll over at 24 bits (~16.7 seconds) must be considered in setting the rxStartTime.
 */
genfskStatus_t GENFSK_StartRx(uint8_t instanceId, uint8_t *pRxBuffer, uint16_t maxBufLengthBytes, GENFSK_timestamp_t rxStartTime, GENFSK_timestamp_t rxDuration);

/*!
 * @brief Performs a receive operation.
 *
 * This function performs a receive operation followed by a transmit operation.
 *
 * @param instanceId The ID of the instance.
 * @param pRxBuffer The pointer to a buffer used for reception.
 * @param maxBufLengthBytes The allocated pBuffer size for the maximum packet length that can be received.
 * @param rxStartTime The time at which to start receive. Set 0 for immediate receive.
 * @param rxDuration The duration of the receive operation. Set 0 for continuous reception.
 * @param pTxBuffer The pointer to a buffer used for transmission in case of AutoAck. Set it to NULL to disable AutoAck.
 * @param txBufLengthBytes The buffer length in bytes. Valid only when AutoAck enabled.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 *
 * @warning Timebase roll over at 24 bits (~16.7 seconds) must be considered in setting the rxStartTime.
 */
genfskStatus_t GENFSK_StartRxTx(uint8_t instanceId, uint8_t *pRxBuffer, uint16_t maxBufLengthBytes, GENFSK_timestamp_t rxStartTime, GENFSK_timestamp_t rxDuration,
                                const uint8_t *pTxBuffer, uint16_t txBufLengthBytes);

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
 */
void GENFSK_AbortAll(void);

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
 * @brief Schedules an event.
 *
 * @param pEvent event to be scheduled.
 *
 * @retval genfskTimerId_t the ID of the allocated timer.
 */
genfskTimerId_t GENFSK_TimeScheduleEvent(GENFSK_TimeEvent_t *pEvent);

/*! 
 * @brief Cancels an event.
 *
 * @param pTimerId pointer to the ID of the timer. Will be reset to gGENFSK_InvalidTimerId_c.
 */
void GENFSK_TimeCancelEvent(genfskTimerId_t *pTimerId);

/*!
 * @brief Converts a packet buffer to a byte array format to be sent by GENFSK LL.
 *
 * This function is used before GENFSK_StartTx() in order to convert the formatted packet in a byte array to be sent over the air.
 * The byte array will have the format :
 *      NWK_ADDRESS | H0 | LENGTH | H1 | PAYLOAD | CRC 
 * This API operates in two modes: "copy" mode and "no copy" mode.
 * - "copy" mode is assumed when pPacket->payload is specified: packet header and payload are copied into pBuffer
 * - "no copy" is assumed when pPacket->payload is NULL. Only packet header is formed into pBuffer, payload has to be copied by the caller.
 *
 * @param instanceId The ID of the instance for which the packet to be formatted.
 * @param pPacket Pointer to the packet structure to be formatted.
 * @param pBuffer Pointer to the byte array formatted buffer.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_PacketToByteArray(uint8_t instanceId, const GENFSK_packet_t *pPacket, uint8_t *pBuffer);

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
genfskStatus_t GENFSK_ByteArrayToPacket(uint8_t instanceId, const uint8_t *pBuffer, GENFSK_packet_t *pPacket);

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

/*!
 * @brief Configures the whitening init register with proper value for BLE. 
 *
 * This function configures the whitening init register with proper value for BLE. When GENFSK_SetChannelNumber() is called
 * for BLE application, this function shall be called too if whitening init is not properly handled by the function itself.
 *
 * @param instanceId The ID of the instance.
 * @param channelNum The channel number on which to transmit and receive, BLE channels from 42 to 120 (included) by step of 2;
 *                   Formula: F = (2360 + channelNum) [in MHz].
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_SetBleWhitenInit(uint8_t instanceId, uint8_t channelNum);


/*! 
 * @brief Returns the xtal trim value (from flash if valid or the default value).
 * 
 * This function returns the xtal trim value (from flash if valid or the default value).
 *
 * @param none.
 *
 * @retval trim value (flash value or default value).
 */
uint32_t GENFSK_GetSavedXtalTrim(void);

#if defined (RADIO_IS_GEN_3P5)
/*!
 * @brief Sets the radio and RBME configurations for the current GENFSK LL instance.
 *
 * This function initializes the radio/RBME and sets the radio/RBME configurations.
 *
 * @param instanceId The ID of the instance.
 * @param radioConfig The radio configuration to be set for GENFSK LL.
 * @param rbmeConf The RBME configuration to be set for GENFSK LL.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_RadioConfigWithRbme(uint8_t instanceId, const GENFSK_radio_config_t *radioConfig, const xcvr_coding_config_t **rbmeConf);
#endif

/*!
 * @brief Promiscuous mode enable.
 *
 * The function enables the promiscuous mode.
 *
 * @note In promiscuous mode various events with failure reasons will be passed. See "genfskEventStatus_t". 
 */
void GENFSK_PromiscuousModeEnable(void);

/*!
 * @brief Promiscuous mode disable.
 *
 * The function disables the promiscuous mode.
 */
void GENFSK_PromiscuousModeDisable(void);

/*!
 * @brief Starts the RX in GTM mode.
 *
 * This function configures and starts the RX in GTM mode. It configures among 
 * other the packet count, the pdu length and the inter packet transmision time.
 *
 * @param instanceId     The ID of the instance.
 *        rx_gtm_config  The RX GTM configuration    
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GtmStartRx(uint8_t instanceId, const GENFSK_gtm_rx_config_t* rx_gtm_config);
/*!
 * @brief Stops the RX in GTM mode.
 *
 * This function stops the RX in GTM mode.    
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GtmStopRx(void);
/*!
 * @brief Starts the TX in GTM mode.
 *
 * This function configures and starts the TX in GTM mode. It configures among 
 * other the packet count, the pdu length, the pdu type and the inter packet transmision time.  
 *
 * @param instanceId     The ID of the instance.
 *        tx_gtm_config  The TX GTM configuration  
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GtmStartTx(uint8_t instanceId, const GENFSK_gtm_tx_config_t* tx_gtm_config);
/*!
 * @brief Stops the TX in GTM mode.
 *
 * This function stops the TX in GTM mode.    
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GtmStopTx(uint8_t instanceId);
/*!
 * @brief Reports the PER statistics.
 *
 * This function reports the PER statistics ( total packet count, good packet count and bad packet count) and tells if
 * activity RX/TX is still enabled or not.
 *
 * @param pkt_results    The PER statistics.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_GtmReadPacketCount(GENFSK_gtm_packet_count_t *pkt_results);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _GENFSK_INTERFACE_H_ */
