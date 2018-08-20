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
#define gGenFskVerPatch_c   6
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
