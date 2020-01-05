/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** 
* @file data_format_hdlc.h
* @brief The data_format_hdlc.h file contains the Host interface definitions and configuration.
*/

#ifndef DATA_FORMAT_HDLC_H_
#define DATA_FORMAT_HDLC_H_

/*******************************************************************************
 * Standard C Includes
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * ISSDK Includes
 ******************************************************************************/
#include "host_io_uart.h"

/*******************************************************************************
 * Types
 ******************************************************************************/
/*! @brief States for receiving a packet */
enum
{ /* Receiver state: Waiting for a packet marker */
  HOST_RX_STATE_WAITFORPACKETMARKER,
  /* Receiver state: Receiving data payload */
  HOST_RX_STATE_GETTINGPACKETDATA
};

/*******************************************************************************
 * APIs
 ******************************************************************************/
/* ===================================================================
 *  @brief       Function to handle incomming HDLC encoded bytes form the Host over UART.
 *  @details     This function will be called on receipt of every UART Byte
 *               and will do the HDLC combination to create a Host Message.
 *  @param[in]   uint8_t c The character in the UART payload.
 *  @param[in]   host_rx_packet_t *pHostRxPkt The Host Packet context structure.
 *  @return      bool Success/Failure.
 *  @constraints This should be the called only after DEBUG/UART has been initialized.
 *  @reeentrant  No
 * =================================================================== */
bool HDLC_Process_Rx_Byte(uint8_t c, host_rx_packet_t *pHostRxPkt);

/* ===================================================================
 *  @brief       Function to format bytes for HDLC to be sent to Host over UART.
 *  @details     This function will encode a Host Protocol formatted Message to be sent to Host over UART.
 *               The encoding adds a start and stop markers and inserts escape sequences for markers inside the packet.
 *  @param[in]   uint8_t *pbuffer The handle to the input buffer containing the Host message.
 *  @param[in]   uint8_t *pMsg    The handle to the output buffer containing the formatted Host message.
 *  @param[in]   size_t size     The number of bytes to be sent starting form the buffer.
 *  @return      size_t          Length of the encoded message.
 *  @constraints This should be the called only after DEBUG/UART has been initialized.
 *  @reeentrant  No
 * =================================================================== */
size_t HDLC_Process_Tx_Msg(const uint8_t *pBuffer, uint8_t *pMsg, size_t size);

#endif // DATA_FORMAT_HDLC_H_
