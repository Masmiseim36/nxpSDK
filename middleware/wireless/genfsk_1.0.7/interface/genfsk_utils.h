/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* SPDX-License-Identifier: BSD-3-Clause
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
/*! @brief Generic FSK build version  */
#define gGenFskVerMajor_c   1
#define gGenFskVerMinor_c   0
#define gGenFskVerPatch_c   7
#define gGenFskBuildNo_c    0

#define gGenFskModuleId_c  0xD0

#define gGenFskVerString_c "Generic FSK v" \
                        QUH(gGenFskVerMajor_c) "." \
                        QUH(gGenFskVerMinor_c) "." \
                        QUH(gGenFskVerPatch_c) "." \
                        QUH(gGenFskBuildNo_c)

/*! @brief GENFSK Packet Buffer base address. */
#define PACKET_BUFFER_BASE_ADDR            (0x4005F700U)
     
/*! @brief GENFSK Promiscuous event enable mask. */
#define gGENFSK_PromiscuousEventMask_c      (0x80000000U)

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
void GENFSK_WritePacketBuffer(uint16_t addr_offset, uint8_t *buffer, uint16_t length);

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

/*! @brief Helper function used by software whitening to reverse the bits of seed and poly values. */
uint16_t GENFSK_Reverse9Bit(uint16_t input);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _GENFSK_UTILS_H_ */
