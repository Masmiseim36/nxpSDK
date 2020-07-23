/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/BusPalPeripheral.h"
#include "blfwk/Logging.h"
#include "blfwk/format_string.h"

using namespace blfwk;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See BusPalUartPeripheral.h for documentation of this method.
BusPalUartPeripheral::BusPalUartPeripheral(const char *port, long speed, const BusPal::BusPalConfigData &config)
    : UartPeripheral(port, speed)
{
    m_busPal = new BusPal(m_fileDescriptor);
    if (m_busPal->enterBitBangMode())
    {
        configure(config);
    }
    else
    {
        delete m_busPal;
        m_busPal = NULL;
        throw std::runtime_error(format_string("Error: BusPalUartPeripheral() cannot enter BitBang Mode."));
    }
}

// See BusPalUartPeripheral.h for documentation of this method.
BusPalUartPeripheral::~BusPalUartPeripheral()
{
}

// See BusPalUartPeripheral.h for documentation of this method.
void BusPalUartPeripheral::configure(const BusPal::BusPalConfigData &config)
{
    switch (config.function)
    {
        case BusPal::kBusPalFunction_SPI:
            if (!m_busPal->enterSpiMode())
            {
                throw std::runtime_error(format_string("Error: BusPalUartPeripheral() cannot enter SPI Mode."));
            }

            if (!m_busPal->setSpiSpeed(config.spiSpeedKHz))
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot set SPI speed(%d kHz).", config.spiSpeedKHz));
            }

            if (!m_busPal->setSpiConfig(config.spiPolarity, config.spiPhase, config.spiDirection))
            {
                throw std::runtime_error(format_string(
                    "Error: BusPalUartPeripheral() cannot set SPI polarity(%s(%d)), phase(%s(%d)), and "
                    "direction(%s(%d)).",
                    config.spiPolarity == BusPal::kSpiClockPolarity_ActiveHigh ? "ActiveHigh" : "ActiveLow",
                    config.spiPolarity,
                    config.spiPhase == BusPal::kSpiClockPhase_FirstEdge ? "FirstEdge" : "SecondEdge", config.spiPhase,
                    config.spiDirection == BusPal::kSpiLsbFirst ? "LsbFirst" : "MsbFirst", config.spiDirection));
            }
            break;

        case BusPal::kBusPalFunction_CAN:
            if (!m_busPal->enterCanMode())
            {
                throw std::runtime_error(format_string("Error: BusPalUartPeripheral() cannot enter CAN Mode."));
            }

            if (!m_busPal->setCanSpeed(config.canSpeed))
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot set CAN speed(%d).", config.canSpeed));
            }

            if (!m_busPal->setCanTxid(config.canTxid))
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot set CAN txid(0x%x).", config.canTxid));
            }

            if (!m_busPal->setCanRxid(config.canRxid))
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot set CAN rxid(0x%x).", config.canTxid));
            }

            break;

        case BusPal::kBusPalFunction_I2C:
            if (!m_busPal->enterI2cMode())
            {
                throw std::runtime_error(format_string("Error: BusPalUartPeripheral() cannot enter I2C Mode."));
            }
            if (!m_busPal->setI2cAddress(config.i2cAddress))
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot set I2C address %02X.", config.i2cAddress));
            }

            if (!m_busPal->setI2cSpeed(config.i2cSpeedKHz))
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot set I2C speed(%d KHz).", config.i2cSpeedKHz));
            }
            break;

        case BusPal::kBusPalFunction_GPIO_CONFIG:
            if (!m_busPal->enterBitBangMode())
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot enter Bitbang mode for GPIO setting."));
            }
            if (!m_busPal->rawConfigurePins(config.gpioPort, config.gpioPinNum, config.gpioMux))
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot configure GPIO %c,%02X,%02X", config.gpioPort,
                                  config.gpioPinNum, config.gpioMux));
            }
            break;

        case BusPal::kBusPalFunction_GPIO_SET:
            if (!m_busPal->enterBitBangMode())
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot enter Bitbang mode for GPIO setting."));
            }
            if (!m_busPal->rawSetPins(config.gpioPort, config.gpioPinNum, config.gpioLevel))
            {
                throw std::runtime_error(format_string("Error: BusPalUartPeripheral() cannot set GPIO %c,%02X,%02X",
                                                       config.gpioPort, config.gpioPinNum, config.gpioLevel));
            }
            break;

        case BusPal::kBusPalFunction_FPGA_CLOCK_SET:
            if (!m_busPal->enterBitBangMode())
            {
                throw std::runtime_error(
                    format_string("Error: BusPalUartPeripheral() cannot enter Bitbang mode for FPGA clock setting."));
            }
            if (!m_busPal->setFPGAClock(config.fpgaClockMhz))
            {
                throw std::runtime_error(format_string("Error: BusPalUartPeripheral() cannot set FPGA clock to %2d MHz",
                                                       config.fpgaClockMhz));
            }
            break;

        default:
            throw std::runtime_error("Unsupported BusPal function type");
    }
}

// See BusPalUartPeripheral.h for documentation of this method.
status_t BusPalUartPeripheral::read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeoutMs)
{
    assert(buffer);

    // Read the requested number of bytes.
    int count = m_busPal->read(buffer, requestedBytes);
    if (actualBytes)
    {
        *actualBytes = count;
    }

    if (count < (int)requestedBytes)
    {
        // Anything less than requestedBytes is a timeout error.
        Log::error("Error: Bus Pal read returned 0\n");
        return kStatus_Timeout;
    }

    return kStatus_Success;
}

// See BusPalUartPeripheral.h for documentation of this method.
status_t BusPalUartPeripheral::write(const uint8_t *buffer, uint32_t byteCount)
{
    assert(buffer);

    const int maxBulk = BusPal::kBulkTransferMax;
    int numBulk = (byteCount / maxBulk);
    int remaining = byteCount - (numBulk * maxBulk);
    bool rc = true;

    // Send buffer in max bulk transfer size chunks.
    for (int i = 0; i < numBulk; ++i)
    {
        rc = m_busPal->write(&buffer[i * maxBulk], maxBulk);
        if (!rc)
        {
            return kStatus_Fail;
        }
    }

    // Send the last OR partial chunk.
    if (rc && remaining)
    {
        rc = m_busPal->write(&buffer[numBulk * maxBulk], remaining);
        if (!rc)
        {
            return kStatus_Fail;
        }
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
