/*
 * The Clear BSD License
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "sai.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;
extern sai_transfer_format_t format;
extern uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM];
extern volatile bool istxFinished;
extern volatile bool isrxFinished;
extern volatile uint32_t beginCount;
extern volatile uint32_t sendCount;
extern volatile uint32_t receiveCount;
extern volatile uint32_t fullBlock;
extern volatile uint32_t emptyBlock;
/*******************************************************************************
 * Code
 ******************************************************************************/
void RecordPlayback(I2S_Type *base, uint32_t time_s)
{
    sai_transfer_t xfer = {0};
    uint32_t playbackCount = 0, recordCount = 0;
    uint32_t txindex = 0, rxindex = 0;

    /* First clear the buffer */
    memset(audioBuff, 0, BUFFER_SIZE * BUFFER_NUM);
    istxFinished = false;
    isrxFinished = false;
    sendCount = 0;
    receiveCount = 0;

    /* Reset SAI internal logic */
    SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);
    SAI_RxSoftwareReset(base, kSAI_ResetTypeSoftware);

    /* Compute the begin count */
    beginCount = time_s * SAMPLE_RATE * 4u / BUFFER_SIZE;

    xfer.dataSize = BUFFER_SIZE;
    /* Wait for playback finished */
    while ((recordCount < beginCount) || (playbackCount < beginCount))
    {
        if ((emptyBlock > 0) && (recordCount < beginCount))
        {
            xfer.data = audioBuff + rxindex * BUFFER_SIZE;
            if (SAI_TransferReceiveEDMA(base, &rxHandle, &xfer) == kStatus_Success)
            {
                rxindex = (rxindex + 1U) % BUFFER_NUM;
                emptyBlock--;
                recordCount++;
            }
        }

        if ((fullBlock > 0) && (playbackCount < beginCount))
        {
            xfer.data = audioBuff + txindex * BUFFER_SIZE;
            if (SAI_TransferSendEDMA(base, &txHandle, &xfer) == kStatus_Success)
            {
                txindex = (txindex + 1U) % BUFFER_NUM;
                fullBlock--;
                playbackCount++;
            }
        }
    }

    /* Wait for record and playback finished */
    while ((istxFinished != true) || (isrxFinished != true))
    {
    }
}
