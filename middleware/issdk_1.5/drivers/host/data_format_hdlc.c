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
* @file data_format_hdlc.c
* @brief The data_format_hdlc.c file contains definitions for
            encoding and decoding HDLC messages to and from Host.
*/

/*******************************************************************************
 * ISSDK Includes
 ******************************************************************************/
#include "data_format_hdlc.h"

/*******************************************************************************
 * Functions
 ******************************************************************************/
/* Function to handle incomming HDLC encoded bytes form the Host over UART. */
bool HDLC_Process_Rx_Byte(uint8_t c, host_rx_packet_t *pHostRxPkt)
{
    static uint8_t c_prev = 0;
    bool bRxPacketerror = false, bPacketReceived = false;

    switch (pHostRxPkt->rxState) /* Process the character. */
    {
        case HOST_RX_STATE_WAITFORPACKETMARKER:
            if (c == 0x7E)
            { /* Wait for the remaining packet. */
                pHostRxPkt->mIndex = 0;
                pHostRxPkt->rxState = HOST_RX_STATE_GETTINGPACKETDATA;
            }
            break;
        case HOST_RX_STATE_GETTINGPACKETDATA:
            if (c != 0x7E)
            { /* Got some real data (non-marker char).
               * Do escape decode if needed. */
                if (c == 0x7D)
                {
                    c_prev = c;
                    break;
                }
                else if (c_prev == 0x7D)
                {
                    if (c == 0x5D)
                    {
                        c = 0x7D;
                    }
                    else if (c == 0x5E)
                    {
                        c = 0x7E;
                    }
                    else
                    { /* Illegal escape sequence.  Reset and wait for next packet. */
                        bRxPacketerror = true;
                    }
                    c_prev = 0;
                }
                /* Escape decode done. Just got data byte, save it. */
                pHostRxPkt->pRxbuf[pHostRxPkt->mIndex] = c;
                ++(pHostRxPkt->mIndex);
            }
            else
            {
                /* Got packet marker while waiting for data. */
                if (pHostRxPkt->mIndex > 0)
                { /* Got a complete packet.
                   * Set state wait for next packet. */
                    pHostRxPkt->rxState = HOST_RX_STATE_WAITFORPACKETMARKER;
                    /* Set flag to process packet just received. */
                    bPacketReceived = true;
                }
                else
                {
                    /* We got back to back packet marker.  This 2nd packet marker will
                     * then be treated as a start marker. Go back to wait for data. */
                    pHostRxPkt->rxState = HOST_RX_STATE_GETTINGPACKETDATA;
                    pHostRxPkt->mIndex = 0;
                }
            }
            break;
        default:
            /* Unknown state. */
            bRxPacketerror = true;
            break;
    }

    /* Handle packet error. */
    if (bRxPacketerror == true)
    {
        pHostRxPkt->mIndex = 0;
        pHostRxPkt->rxState = HOST_RX_STATE_WAITFORPACKETMARKER;
    }

    return bPacketReceived;
}

/* Function to format data for HDLC and send bytes to Host over UART. */
size_t HDLC_Process_Tx_Msg(const uint8_t *pBuffer, uint8_t *pMsg, size_t size)
{
    size_t index = 0;

    if (pBuffer == NULL || pMsg == NULL || size == 0)
    {
        return 0;
    }

    /* Construct the Host Message. */
    pMsg[index++] = 0x7E; /* Add the Start marker. */
    for (size_t offset = 0; offset < size; offset++)
    {
        switch (pBuffer[offset])
        {
            case 0x7D: /* Add escape sequence for escape character. */
                pMsg[index++] = 0x7D;
                pMsg[index++] = 0x5D;
                break;
            case 0x7E: /* Add escape sequence for start/stop character. */
                pMsg[index++] = 0x7D;
                pMsg[index++] = 0x5E;
                break;
            default: /* Add the actual character. */
                pMsg[index++] = pBuffer[offset];
        }
    }
    pMsg[index++] = 0x7E; /* Add the Stop marker. */

    return index;
}
