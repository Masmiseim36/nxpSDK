/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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

#include "mu_transport.h"
#include "erpc_config_internal.h"
#include "board.h"

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static MUTransport *s_mu_instance = NULL;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
void MUTransport::mu_tx_empty_irq_callback()
{
    MUTransport *transport = s_mu_instance;
    if ((transport) && (transport->m_muBase->CR & (1U << (MU_CR_TIEn_SHIFT + MU_TR_COUNT - MU_REG_COUNT))))
    {
        transport->tx_cb();
    }
}

void MUTransport::mu_rx_full_irq_callback()
{
    MUTransport *transport = s_mu_instance;
    if ((transport) && (transport->m_muBase->CR & (1U << (MU_CR_RIEn_SHIFT + MU_RR_COUNT - MU_REG_COUNT))))
    {
        transport->rx_cb();
    }
}

MUTransport::MUTransport()
: Transport()
, m_newMessage(false)
, m_rxMsgSize(0)
, m_rxCntBytes(0)
, m_rxBuffer(NULL)
, m_txMsgSize(0)
, m_txCntBytes(0)
, m_txBuffer(NULL)
#if ERPC_THREADS
, m_rxSemaphore()
, m_txSemaphore()
, m_sendLock()
, m_receiveLock()
#endif
, m_muBase(NULL)
{
    s_mu_instance = this;
}

MUTransport::~MUTransport()
{
}

erpc_status_t MUTransport::init(MU_Type *muBase)
{
    m_muBase = muBase;

#if !ERPC_THREADS
    // enabling the MU rx full irq is necessary only for BM app
    MU_EnableInterrupts(m_muBase, (1U << (MU_CR_RIEn_SHIFT + MU_RR_COUNT - MU_REG_COUNT)));
#endif

    NVIC_SetPriority(MU_IRQ, MU_IRQ_PRIORITY);
    EnableIRQ(MU_IRQ);

    return kErpcStatus_Success;
}

void MUTransport::rx_cb()
{
    if (m_rxBuffer == NULL)
    {
        // the receive function has not been called yet
        // disable MU rx full interrupt
        MU_DisableInterrupts(m_muBase, (1U << (MU_CR_RIEn_SHIFT + MU_RR_COUNT - MU_REG_COUNT)));
        m_newMessage = true;
        return;
    }

    // read data from the MU rx registers
    uint32_t i = 0;
    if (m_rxMsgSize == 0)
    {
        m_rxMsgSize = MU_ReceiveMsgNonBlocking(m_muBase, i++);
    }

    for (; i < MU_REG_COUNT; i++)
    {
        if (m_rxCntBytes < m_rxMsgSize)
        {
            m_rxBuffer[m_rxCntBytes >> 2] = MU_ReceiveMsgNonBlocking(m_muBase, i);
            m_rxCntBytes += 4;
        }
        else
        {
            // read MU rx reg to clear the rx full flag
            MU_ReceiveMsgNonBlocking(m_muBase, i);
        }
    }

    // message is complete, unblock caller of the receive function
    if (m_rxCntBytes >= m_rxMsgSize)
    {
        m_rxBuffer = NULL;
#if ERPC_THREADS
        MU_DisableInterrupts(m_muBase, (1U << (MU_CR_RIEn_SHIFT + MU_RR_COUNT - MU_REG_COUNT)));
        m_rxSemaphore.putFromISR();
#endif
    }
}

void MUTransport::tx_cb()
{
    // fill MU tx regs
    uint32_t i = 0;
    for (i = 0; i < MU_REG_COUNT; i++)
    {
        // prepare uint32_t and write it to next MU tx reg
        uint32_t tx = 0;
        if (m_txCntBytes < m_txMsgSize)
        {
            tx = m_txBuffer[m_txCntBytes >> 2];
        }
        MU_SendMsgNonBlocking(m_muBase, i, tx);
        m_txCntBytes += 4;
    }

    // if the message was sent whole, disable the MU tx empty irq
    if (m_txCntBytes >= m_txMsgSize)
    {
        // disable MU tx empty irq from the last tx reg
        MU_DisableInterrupts(m_muBase, (1U << (MU_CR_TIEn_SHIFT + MU_TR_COUNT - MU_REG_COUNT)));

        // unblock caller of the send function
        m_txBuffer = NULL;
#if ERPC_THREADS
        m_txSemaphore.putFromISR();
#endif
    }
}

erpc_status_t MUTransport::receive(MessageBuffer *message)
{
    if (message == NULL)
    {
        return kErpcStatus_SendFailed;
    }

#if ERPC_THREADS
    Mutex::Guard lock(m_receiveLock);
#endif

    m_rxMsgSize = 0;
    m_rxCntBytes = 0;
    m_rxBuffer = (uint32_t *)message->get();

    // enable the MU rx full irq
    MU_EnableInterrupts(m_muBase, (1U << (MU_CR_RIEn_SHIFT + MU_RR_COUNT - MU_REG_COUNT)));

// wait until the receiving is not complete
#if ERPC_THREADS
    m_rxSemaphore.get();
#else
    while (m_rxBuffer)
    {
    }
#endif

    message->setUsed(m_rxMsgSize);
    m_newMessage = false;

    return kErpcStatus_Success;
}

erpc_status_t MUTransport::send(MessageBuffer *message)
{
    if (message == NULL)
    {
        return kErpcStatus_SendFailed;
    }

#if ERPC_THREADS
    Mutex::Guard lock(m_sendLock);
#endif

    m_txMsgSize = message->getUsed();
    m_txCntBytes = 0;
    m_txBuffer = (uint32_t *)message->get();

    MU_SendMsgNonBlocking(m_muBase, 0, m_txMsgSize);

    // write to next MU tx registers
    uint8_t i = 0;
    for (i = 1; i < MU_REG_COUNT; i++)
    {
        // prepare next uint32_t and write it to next MU tx reg
        uint32_t tx = 0;
        if (m_txCntBytes < m_txMsgSize)
        {
            tx = m_txBuffer[m_txCntBytes >> 2];
        }
        MU_SendMsgNonBlocking(m_muBase, i, tx);
        m_txCntBytes += 4;
    }

    // if the message wasn't sent whole, enable the MU tx empty irq
    if (m_txCntBytes < m_txMsgSize)
    {
        // enable MU tx empty irq from the last mu tx reg
        MU_EnableInterrupts(m_muBase, (1U << (MU_CR_TIEn_SHIFT + MU_TR_COUNT - MU_REG_COUNT)));
// wait until the sending is not complete
#if ERPC_THREADS
        m_txSemaphore.get();
#else
        while (m_txBuffer)
        {
        }
#endif
    }
    return kErpcStatus_Success;
}

extern "C" {

/*!
 * @brief Messaging Unit TxEmptyFlag ISR handler
 *
 * This function overloads the weak handler defined in MCMGR MU_interrupts[] MU ISR table
 */
void MU_TxEmptyFlagISRCallback()
{
    MUTransport::mu_tx_empty_irq_callback();
}

/*!
 * @brief Messaging Unit RxFullFlag ISR handler
 *
 * This function overloads the weak handler defined in MCMGR MU_interrupts[] MU ISR table
 */
void MU_RxFullFlagISRCallback()
{
    MUTransport::mu_rx_full_irq_callback();
}
}
