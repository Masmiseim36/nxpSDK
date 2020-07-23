/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#ifndef _GENFSK_UTILS_H_
#define _GENFSK_UTILS_H_

#include "EmbeddedTypes.h"

/*!
 * @addtogroup genfsk_utils
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief GENFSK Packet Buffer base address. */
#if (defined(CPU_MKW21Z256VHT4) || defined(CPU_MKW21Z512VHT4) || defined(CPU_MKW31Z256CAx4) || \
     defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAx4) || defined(CPU_MKW31Z512VHT4) || \
     defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512VHT4) || \
     defined(CPU_MKW35A512VFP4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW36A512VFP4) || \
     defined(CPU_MKW36A512VHT4) || defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4))
#define PACKET_BUFFER_BASE_ADDR            (0x4005F700U)
#elif (defined(CPU_K32W042S1M2VPJ_cm0plus))
#define PACKET_BUFFER_BASE_ADDR            (0x41033700U)
#endif

#ifndef RX_PACKET_RAM_BASE
#define RX_PACKET_RAM_BASE PACKET_BUFFER_BASE_ADDR
#endif

#ifndef TX_PACKET_RAM_BASE
#define TX_PACKET_RAM_BASE PACKET_BUFFER_BASE_ADDR
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name GENFSK Utils
 * @{
 */

/*!
 * @brief Writes data into the packet buffer.
 *
 * This function writes data into the GENFSK packet buffer memory region.
 *
 * @note This function does not check for invalid address offset, buffer pointer or data
 * length. 
 *
 * @param addr_offset The address offset in packet buffer from where to start the write procedure.
 * @param buffer Start address of the data to write.
 * @param length Size of the data to write.
 */
void GENFSK_WritePacketBuffer(uint16_t addr_offset, const uint8_t *buffer, uint16_t length);

/*!
* @brief Reads data from the GENFSK packet buffer.
 *
 * This function reads data from the GENFSK packet buffer.
 *
 * @param addr_offset The address offset in packet buffer from where to start reading.
 * @param buffer Start address of the buffer to store the received data.
 * @param length Size of the data to be received.
 */
void GENFSK_ReadPacketBuffer(uint16_t addr_offset, uint8_t *buffer, uint16_t length);

/*!
* @brief Set an arbitrary frequency for RX and TX for the radio.
 *
 * This function sets the radio frequency used for RX and TX.
 *
 * @param frequency Target frequency setting in Hz.
 *
 * @retval gGenfskSuccess_c if success or the failure reason.
 */
genfskStatus_t GENFSK_OverrideFrequency(uint32_t frequency);

/*! @brief Helper function used by software whitening to reverse the bits of seed and poly values. */
uint16_t GENFSK_Reverse9Bit(uint16_t input);

/*! @brief Helper function that computes the TX duration of a packet 
 * 
 * @param instanceId The ID of the instance.
 * @param nBytes The number of bytes to transmit.
 *
 * @retval frame tx duration in us
 */
uint32_t GENFSK_GetTxDuration(uint8_t instanceId, uint16_t nBytes);

/*! @brief Helper function that sets the xtal trim if valid 
 * 
 * @param xtalTrim The xtal trim value.
 *
 * @retval genfsk status
 */
genfskStatus_t GENFSK_SetXtalTrim(uint32_t xtalTrim);

#if defined(gGenfskPreserveXcvrDacTrimValue_d) && (gGenfskPreserveXcvrDacTrimValue_d == 1)
/*! @brief Helper function that save the radio calibration in flash
 *
 * @param xcvrDacTrim radio calibration to save
 *
 */
void GENFSK_SaveXcvrDcocDacTrimToFlash(xcvr_DcocDacTrim_t *xcvrDacTrim);

/*! @brief Helper function that restore radio calibration from flash
 *
 * @param xcvrDacTrim Pointer where to store radio calibration.
 *
 * @retval TRUE if XCVR trim is invalid, false if it is valid
 */
bool_t GENFSK_RestoreXcvrDcocDacTrimFromFlash(xcvr_DcocDacTrim_t *xcvrDacTrim);
#endif

#ifndef RADIO_IS_GEN_3P5
extern void GENFSK_MskPreProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length, uint8_t initBit);
extern void GENFSK_MskPostProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint16_t length, uint8_t initBit, uint8_t lsbToMsb);
#endif

/*! @brief Initializes the GENFSK timer module. */
void GENFSK_TimeInit(void);

/*! @brief Disables GENFSK timer T2 timeout. */ 
void GENFSK_TimeDisableWaitTimeout(void);

/*!
 * @brief Returns a 64bit timestamp value.
 *
 * @retval GENFSK_timestamp_t timestamp.
 */
GENFSK_timestamp_t GENFSK_TimeGetTimestamp(void);

/*! @brief GENFSK Interrupt install and enable. */
void GENFSK_InstallInterrupt(void);

extern void GENFSK_EnableInterrupts(uint32_t mask);
extern void GENFSK_DisableInterrupts(uint32_t mask);

/*! @brief Disable GENFSK global interrupt. */
extern void GENFSK_ProtectFromXcvrInterrupt(void);

/*! @brief Enable GENFSK global interrupt. */
extern void GENFSK_UnprotectFromXcvrInterrupt(void);

/*! @brief GENFSK time interrupt service routine. */ 
void GENFSK_TimeISR(void);

void GENFSK_SwapBytes(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length);

extern volatile uint64_t gGenfskTimerOverflow;

/*! @} */

#if !defined (RADIO_IS_GEN_3P5)
genfskStatus_t GENFSK_GetXcvrConfig(genfskRadioMode_t radioModeIn, radio_mode_t *radioMode);
#else
genfskStatus_t GENFSK_GetXcvrConfig(genfskRadioMode_t radioModeIn, genfskDataRate_t dataRate, const xcvr_config_t **xcvrConfig);
genfskStatus_t GENFSK_ConfigurePower(uint8_t isHighPwrReq);
uint8_t GENFSK_IsHighPowerConfigured(void);
#endif

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _GENFSK_UTILS_H_ */
