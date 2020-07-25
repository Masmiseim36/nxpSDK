/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/BusPal.h"
#include "blfwk/Logging.h"
#include "blfwk/serial.h"
#include <algorithm>

#ifdef LINUX
#include <cstring>
#include <unistd.h>
#endif

using namespace blfwk;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Bit Bang Mode commands.
enum
{
    kBitBang_Reset = 0x00,        //!< Reset, responds "BBIO1"
    kBitBang_SpiMode = 0x01,      //!< Enter binary SPI mode, responds "SPI1"
    kBitBang_I2cMode = 0x02,      //!< Enter binary I2C mode, responds "I2C1"
    kBitBang_UartMode = 0x03,     //!< Enter binary UART mode, responds "ART1"
    kBitBang_1Wire = 0x04,        //!< Enter binary 1-Wire mode, responds "1W01"
    kBitBang_RawWire = 0x05,      //!< Enter binary raw-wire mode, responds "RAW1"
    kBitBang_Jtag = 0x06,         //!< Enter OpenOCD JTAG mode
    kBitBang_CanMode = 0x07,      //!< Enter binary CAN mode, responds "CAN1"
    kBitBang_HardReset = 0x0f,    //!< Reset Bus Pal
    kBitBang_SelfTest = 0x10,     //!< Bus Pal self-tests
    kBitBang_SetupPwm = 0x12,     //!< Setup pulse-width modulation (requires 5 byte setup)
    kBitBang_ClearPwm = 0x13,     //!< Clear/disable PWM
    kBitBang_Probe = 0x14,        //!< Take voltage probe measurement (returns 2 bytes)
    kBitBang_ContProbe = 0x15,    //!< Continuous voltage probe measurement
    kBitBang_FreqAux = 0x16,      //!< Frequency measurement on AUX pin
    kBitBang_SetFpgaClock = 0x20, //!< Set the clock frequency on the FPGA board
    kBitBang_CfgPins = 0x40,      //!< Configure pins as input(1) or output(0): AUX|MOSI|CLK|MISO|CS
    kBitBang_SetPins = 0x80       //!< Set on (1) or off (0): POWER|PULLUP|AUX|MOSI|CLK|MISO|CS
};

//! @brief Uart Mode commands.
enum
{
    kUart_Version = 0x01,      //!< Display mode version string, responds "ARTx"
    kUart_EchoRx = 0x02,       //!< Start (0)/stop(1) echo UART RX
    kUart_SetBaud = 0x07,      //!< Manual baud rate configuration, send 2 bytes
    kUart_Bridge = 0x0f,       //!< UART bridge mode (reset to exit)
    kUart_BulkWrite = 0x10,    //!< Bulk UART write, send 1-16 bytes (0=1byte!)
    kUart_ConfigPeriph = 0x40, //!< Configure peripherals w=power, x=pullups, y=AUX, z=CS
    kUart_SetSpeed = 0x60,     //!< Set UART speed
    kUart_ConfigUart = 0x80    //!< Configure UART settings
};

//! @brief Spi mode commands.
enum
{
    kSpi_Exit = 0x00,         //!< 00000000 - Exit to bit bang mode
    kSpi_Version = 0x01,      //!< 00000001 - Enter raw SPI mode, display version string
    kSpi_ChipSelect = 0x02,   //!< 0000001x - CS high (1) or low (0)
    kSpi_Sniff = 0x0c,        //!< 000011XX - Sniff SPI traffic when CS low(10)/all(01)
    kSpi_BulkTransfer = 0x10, //!< 0001xxxx - Bulk SPI transfer, send/read 1-16 bytes (0=1byte!)
    kSpi_ConfigPeriph = 0x40, //!< 0100wxyz - Configure peripherals w=power, x=pull-ups, y=AUX, z=CS
    kSpi_SetSpeed = 0x60,     //!< 01100xxx - SPI speed
    kSpi_ConfigSpi = 0x80,    //!< 1000wxyz - SPI config, w=HiZ/3.3v, x=CKP idle, y=CKE edge, z=SMP sample
    kSpi_WriteThenRead = 0x04 //!< 00000100 - Write then read extended command
};

//! @brief Spi configuration shifts for the mask
enum
{
    kSpiConfigShift_Direction = 0,
    kSpiConfigShift_Phase = 1,
    kSpiConfigShift_Polarity = 2
};

//! @brief Can mode commands.
enum
{
    kCan_Exit = 0x00,          //!< 00000000 - Exit to bit bang mode
    kCan_Version = 0x01,       //!< 00000001 - Display mode version string, responds "CANx"
    kCan_BulkWrite = 0x02,     //!< 00000010
    kCan_WriteThenRead = 0x03, //!< 00000011 - Write then read extended command
    kCan_ReadFrame = 0x04,     //!< 00000100 - CAN read frame
    kCan_WriteFrame = 0x05,    //!< 00000101 - CAN write frame
    kCan_AckBit = 0x06,        //!< 00000110 - ACK bit
    kCan_NackBit = 0x07,       //!< 00000111 - NACK bit
    kCan_SetRxFifoId = 0x10,   //!< 00010000 - Set CAN RX FIFO id
    kCan_SetRxMbId = 0x20,     //!< 00100000 - Set CAN RX MB id
    kCan_SetTxMbId = 0x30,     //!< 00110000 - Set CAN TX MB id
    kCan_SetSpeed = 0x40       //!< 010000xx - Set CAN speed, 0=125kHz, 1=250kHz, 2=500kHz, 3=750kHz, 4=1MHz
};

enum
{
    kCanMaxReadAttemptCount = 3 //!< CAN reads will sometimes fail, this will be the retry count
};

#pragma pack(1)
struct SpiWriteThenReadCommand
{
    uint8_t command;
    uint16_t numBytesToWrite;
    uint16_t numBytesToRead;
};

struct CanWriteThenReadCommand
{
    uint8_t command;
    uint16_t numBytesToWrite;
    uint16_t numBytesToRead;
};

struct SpiSetSpeedCommand
{
    uint8_t command;
    uint32_t speed;
};

struct CanSetTxidCommand
{
    uint8_t command;
    uint32_t txid;
};

struct CanSetRxidCommand
{
    uint8_t command;
    uint32_t rxid;
};

struct CanSetSpeedCommand
{
    uint8_t command;
    uint32_t speed;
};

struct I2cSetSpeedCommand
{
    uint8_t command;
    uint32_t speed;
};

struct I2cWriteThenReadCommand
{
    uint8_t command;
    uint16_t numBytesToWrite;
    uint16_t numBytesToRead;
};

struct GpioConfigCommand
{
    uint8_t command;
    uint8_t port;
    uint8_t pin;
    uint8_t muxVal;
};

struct GpioSetCommand
{
    uint8_t command;
    uint8_t port;
    uint8_t pin;
    uint8_t level;
};

struct SetFpgaClockCommand
{
    uint8_t command;
    uint32_t speed;
};
#pragma pack()

//! @brief I2c mode commands.
enum
{
    kI2c_Exit = 0x00,            //!< 00000000 - Exit to bit bang mode
    kI2c_Version = 0x01,         //!< 00000001 - Display mode version string, responds "I2Cx"
    kI2c_StartBit = 0x02,        //!< 00000010 - I2C start bit
    kI2c_StopBit = 0x03,         //!< 00000011 - I2C stop bit
    kI2c_ReadByte = 0x04,        //!< 00000100 - I2C read byte
    kI2c_AckBit = 0x06,          //!< 00000110 - ACK bit
    kI2c_NackBit = 0x07,         //!< 00000111 - NACK bit
    kI2c_BusSniff = 0x0F,        //!< 00001111 - Start bus sniffer
    kI2c_BulkWrite = 0x10,       //!< 0001xxxx - Bulk I2C write, send 1-16 bytes (0=1byte!)
    kI2c_ConfigurePeriph = 0x40, //!< 0100wxyz - Configure peripherals w=power, x=pullups, y=AUX, z=CS
    kI2c_PullUpSelect = 0x50,    //!< 010100xy - Pull up voltage select (BPV4 only)- x=5v y=3.3v
    kI2c_SetSpeed =
        0x60, //!< 011000xx - Set I2C speed, 3=~400kHz, 2=~100kHz, 1=~50kHz, 0=~5kHz (updated in v4.2 firmware)
    kI2c_SetAddress = 0x70,   //!< 11100000 - Set I2C address
    kI2c_WriteThenRead = 0x08 //!< Write then read
};

enum
{
    kSerialReadTimeoutMs = 3000, //!< 3 second timeout
    kI2cMaxReadAttemptCount = 2  //!< I2c reads will sometimes fail, this will be the retry count
};

//! @name Expected mode enter command responses.
//@{

//! @brief Response to enter bit bang mode.
const char k_responseBitBangMode[] = "BBIO1";
const char k_responseReset[] = "BBIO1";
const char k_responseSpiMode[] = "SPI1";
const char k_responseCanMode[] = "CAN1";
const char k_responseI2cMode[] = "I2C1";
const char k_responseUartMode[] = "ART1";

//@}

//! @brief UART settings.
//!
//! Arguments (lower 5 bits) are wxxyz:
//! w=pin output HiZ(0)/3.3v(1)
//! xx=databits and parity 8/N(0), 8/E(1), 8/O(2), 9/N(3)
//! y=stop bits 1(0)/2(1)
//! z=RX polarity idle 1 (0), idle 0 (1)
enum
{
    kUartSetting_33 = 0x10,
    kUartSettings = kUartSetting_33
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
void BusPal::flushRX()
{
    uint8_t readBuf[100];
    Log::info("Flushing data...\n");
    int test;
    while ((test = buspal_serial_read(readBuf, sizeof(readBuf))))
    {
        if (test)
        {
            Log::info("Flushed %d bytes\n", test);
        }
    }

    Log::info("Done flushing.\n");
}

// See See bus_pal.h for documentation on this method.
BusPal::BusPal(int fileDescriptor)
    : m_fileDescriptor(fileDescriptor)
    , m_mode(kBusPalModeBitBang)
    , m_spiWriteByteCount(0)
    , m_canFirstReadDelay(100)
    , m_canWriteByteCount(0)
{
}

bool BusPal::parse(const string_vector_t &params, BusPal::BusPalConfigData &config)
{
    if (!params[0].compare(0, 3, "spi"))
    {
        config.function = BusPal::kBusPalFunction_SPI;

        if ((params.size() > 1))
        {
            int32_t spiSpeed = atoi(params[1].c_str());
            if (spiSpeed <= 0)
                return false;

            config.spiSpeedKHz = spiSpeed;

            if (params.size() > 2)
            {
                config.spiPolarity = (BusPal::spi_clock_polarity_t)atoi(params[2].c_str());

                if (params.size() > 3)
                {
                    config.spiPhase = (BusPal::spi_clock_phase_t)atoi(params[3].c_str());

                    if (params.size() > 4)
                    {
                        if (!strcmp(params[4].c_str(), "lsb"))
                        {
                            config.spiDirection = BusPal::kSpiLsbFirst;
                        }
                        else if (!strcmp(params[4].c_str(), "msb"))
                        {
                            config.spiDirection = BusPal::kSpiMsbFirst;
                        }
                    }
                }
            }
        }
    }
    else if (!params[0].compare(0, 3, "i2c"))
    {
        config.function = BusPal::kBusPalFunction_I2C;

        if (params.size() > 1)
        {
            uint32_t i2cAddress = strtoul(params[1].c_str(), NULL, 16);

            if (i2cAddress > 0x7F)
            {
                i2cAddress &= 0x7F;
                Log::info("Only 7-bit i2c address is supported, so the effective value is 0x%x\n", i2cAddress);
            }
            config.i2cAddress = (uint8_t)i2cAddress;

            if (params.size() > 2)
            {
                int32_t i2cSpeed = atoi(params[2].c_str());
                if (i2cSpeed <= 0)
                    return false;

                config.i2cSpeedKHz = i2cSpeed;
            }
        }
    }
    else if (!params[0].compare(0, 3, "can"))
    {
        config.function = BusPal::kBusPalFunction_CAN;
        if ((params.size() > 1))
        {
            uint32_t canSpeed = atoi(params[1].c_str());
            if (canSpeed > 4)
                return false;

            config.canSpeed = canSpeed;

            if (params.size() > 2)
            {
                config.canTxid = strtoul(params[2].c_str(), NULL, 16) & 0x7ff;
            }

            if (params.size() > 3)
            {
                config.canRxid = strtoul(params[3].c_str(), NULL, 16) & 0x7ff;
            }
        }
    }
    else if (!params[0].compare(0, 4, "gpio"))
    {
        if (!params[1].compare(0, 6, "config"))
        {
            if (params.size() != 5)
            {
                return false;
            }
            config.function = kBusPalFunction_GPIO_CONFIG;
            config.gpioPort = (uint8_t)*params[2].c_str();
            config.gpioPinNum = atoi(params[3].c_str());
            config.gpioMux = atoi(params[4].c_str());
        }
        else if (!params[1].compare(0, 3, "set"))
        {
            if (params.size() != 5)
            {
                return false;
            }
            config.function = kBusPalFunction_GPIO_SET;
            config.gpioPort = (uint8_t)*params[2].c_str();
            config.gpioPinNum = atoi(params[3].c_str());
            config.gpioLevel = atoi(params[4].c_str());
        }
        else
        {
            return false;
        }
    }
    else if (!params[0].compare(0, 5, "clock"))
    {
        if (params.size() != 2)
        {
            return false;
        }
        config.function = kBusPalFunction_FPGA_CLOCK_SET;
        config.fpgaClockMhz = atoi(params[1].c_str());
    }
    else
    {
        // Error: Invalid BusPal function.
        return false;
    }

    return true;
}

bool BusPal::enterSpiMode()
{
    bool retVal = writeCommand(kBitBang_SpiMode, k_responseSpiMode);

    if (retVal)
    {
        m_mode = kBusPalModeSpi;
    }

    return retVal;
}

bool BusPal::enterCanMode()
{
    bool retVal = writeCommand(kBitBang_CanMode, k_responseCanMode);

    if (retVal)
    {
        m_mode = kBusPalModeCan;
    }

    return retVal;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setCanTxid(unsigned int txid)
{
    CanSetTxidCommand command = { kCan_SetTxMbId, txid };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to Can Set Txid, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setCanRxid(unsigned int rxid)
{
    CanSetRxidCommand command = { kCan_SetRxMbId, rxid };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to Can Set Rxid, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setCanSpeed(unsigned int speed)
{
    CanSetSpeedCommand command = { kCan_SetSpeed, speed };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to Can Set Speed, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

bool BusPal::enterI2cMode()
{
    bool retVal = writeCommand(kBitBang_I2cMode, k_responseI2cMode);

    if (retVal)
    {
        m_mode = kBusPalModeI2c;
    }

    return retVal;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::enterBitBangMode()
{
    Log::info("Entering bit bang mode...\n");

    if (reset())
    {
        Log::info("Entered BB mode\n");
        return true;
    }
    else
    {
        Log::error("Error: enter bit bang mode failed\n");
        Log::error("Dumping any remaining data...\n");

        uint8_t blah;
        while (buspal_serial_read(&blah, 1))
            ;

        return false;
    }
}

// See See bus_pal.h for documentation on this method.
bool BusPal::writeCommand(uint8_t commandByte, const char *expectedResponse)
{
    assert(expectedResponse);
    int actualBytes = buspal_serial_write(&commandByte, 1, true);
    if (actualBytes != 1)
    {
        Log::error("Error: Bus Pal write command failed\n");
        return false;
    }

    char *str = 0;
    unsigned int count = strlen(expectedResponse);

    while (!(str = reinterpret_cast<char *>(response(count))))
        ;

    return (strncmp(str, expectedResponse, count) == 0);
}

// See See bus_pal.h for documentation on this method.
uint8_t BusPal::writeCommand(uint8_t commandByte)
{
    int actualBytes = buspal_serial_write(&commandByte, 1, true);
    if (actualBytes != 1)
    {
        Log::error("Error: Bus Pal write command failed\n");
        return false;
    }

    uint8_t *data = 0;

    while (data == NULL)
    {
        data = response();
    }

    return *data;
}

uint8_t BusPal::writeCommand(uint8_t *command, unsigned int length)
{
    int actualBytes = buspal_serial_write(command, length, true);
    if (actualBytes != length)
    {
        Log::error("Error: Bus Pal write command failed\n");
        return false;
    }

    uint8_t *data = 0;

    while (data == NULL)
    {
        data = response();
    }

    return *data;
}

// See See bus_pal.h for documentation on this method.
uint8_t *BusPal::response(size_t byteCount)
{
    int remainingBytes = std::min<int>(byteCount, sizeof(m_responseBuffer));
    uint8_t *buffer = m_responseBuffer;

    while (remainingBytes > 0)
    {
        int actualBytes = buspal_serial_read(buffer, remainingBytes, true);
        if (actualBytes > 0)
        {
            remainingBytes -= actualBytes;
            buffer += actualBytes;
        }
        else
        {
            // If no bytes received, it is a timeout error.
            return NULL;
        }
    }
    return m_responseBuffer;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::resetHardware()
{
    uint8_t rc = writeCommand(kBitBang_HardReset);
    if (rc != kResponseOk)
    {
        Log::error("Error: bad response from hard reset\n");
        return false;
    }
    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::reset()
{
    // We need to do a non wait forever loop here to verify that bus pal is running/connected,
    // so that is why we are doing this manually instead of using writeCommand
    // if bus pal is not active this will return failure after the serial read timeout instead of blocking forever
    // Reset is always the first command run when talking to bus pal
    uint8_t commandByte = kBitBang_Reset;
    int actualBytes = buspal_serial_write(&commandByte, sizeof(commandByte), true);
    bool retVal = false;
    if (actualBytes == sizeof(commandByte))
    {
        uint8_t reply[sizeof(k_responseBitBangMode) - 1]; // reply does not send a null
        actualBytes = buspal_serial_read(reply, sizeof(reply), true);

        if (actualBytes == (sizeof(k_responseBitBangMode) - 1))
        {
            if (!strncmp(reinterpret_cast<const char *>(reply), k_responseBitBangMode,
                         sizeof(k_responseBitBangMode) - 1))
            {
                retVal = true;
            }
        }
    }

    return retVal;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setSpiConfig(spi_clock_polarity_t polarity, spi_clock_phase_t phase, spi_shift_direction_t direction)
{
    uint8_t mask = (polarity & 1) << kSpiConfigShift_Polarity;
    mask |= (phase & 1) << kSpiConfigShift_Phase;
    mask |= (direction & 1) << kSpiConfigShift_Direction;

    uint8_t rc = writeCommand(kSpi_ConfigSpi | mask);

    if (rc != kResponseOk)
    {
        Log::error("Error: bad response from Set Spi Config\n");
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setSpiSpeed(unsigned int speed)
{
    SpiSetSpeedCommand command = { kSpi_SetSpeed, speed };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to Spi Set Speed, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setI2cAddress(uint8_t address)
{
    uint8_t command[] = { kI2c_SetAddress, address };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to Set I2c Address, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setI2cSpeed(uint32_t speed)
{
    I2cSetSpeedCommand command = { kI2c_SetSpeed, speed };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to I2c Set Speed, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}
// See See bus_pal.h for documentation on this method.
bool BusPal::rawConfigurePins(uint8_t port, uint8_t pin, uint8_t muxVal)
{
    GpioConfigCommand command = { kBitBang_CfgPins, port, pin, muxVal };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to Gpio Set, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::rawSetPins(uint8_t port, uint8_t pin, uint8_t level)
{
    GpioSetCommand command = { kBitBang_SetPins, port, pin, level };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to Gpio Set, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::setFPGAClock(uint32_t clock)
{
    SetFpgaClockCommand command = { kBitBang_SetFpgaClock, clock };

    uint8_t respData = writeCommand(reinterpret_cast<uint8_t *>(&command), sizeof(command));

    if (respData != kResponseOk)
    {
        Log::error("Error: bad response to FPGA Set Clock, response byte = 0x%x\n", respData);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::write(const uint8_t *data, size_t byteCount)
{
    switch (m_mode)
    {
        case kBusPalModeSpi:
            return writeSpi(data, byteCount);
        case kBusPalModeI2c:
            return writeI2c(data, byteCount);
        case kBusPalModeCan:
            return writeCan(data, byteCount);
        default:
            return false;
    }
}

// See See bus_pal.h for documentation on this method.
int BusPal::read(uint8_t *data, size_t byteCount)
{
    switch (m_mode)
    {
        case kBusPalModeSpi:
            return readSpi(data, byteCount);
        case kBusPalModeCan:
            return readCan(data, byteCount);
        case kBusPalModeI2c:
        {
            if (!byteCount)
            {
                return 0;
            }

            int attemptCount = 0;
            int bytesRead = 0;

            while (!bytesRead && (attemptCount++ < kI2cMaxReadAttemptCount))
            {
                bytesRead = readI2c(data, byteCount);
                if (!bytesRead)
                {
                    Log::info("Initial read on I2C failed, trying again...\n");
                }
            }

            return bytesRead;
        }
        default:
            return false;
    }
}

// See See bus_pal.h for documentation on this method.
bool BusPal::writeSpi(const uint8_t *data, size_t byteCount)
{
    assert(data);

    SpiWriteThenReadCommand command;
    command.command = kSpi_WriteThenRead;
    command.numBytesToWrite = std::min<int>(byteCount, kBulkTransferMax);
    command.numBytesToRead = 0;

    m_spiWriteByteCount += byteCount;

    int actualBytes = buspal_serial_write(reinterpret_cast<uint8_t *>(&command), sizeof(command), true);
    if (actualBytes != sizeof(command))
    {
        Log::error("Error: Bus Pal spi write command failed\n");
        return false;
    }

    actualBytes = buspal_serial_write(const_cast<uint8_t *>(data), byteCount);
    if (actualBytes != byteCount)
    {
        Log::error("Error: Bus Pal spi write command failed\n");
        return false;
    }

    uint8_t *respData = 0;

    while (respData == NULL)
    {
        respData = response(1);
    }

    if (respData[0] != kResponseOk)
    {
        Log::error("Error: bad response to spi write, response byte = 0x%x\n", respData[0]);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
int BusPal::readSpi(uint8_t *data, size_t byteCount)
{
    return readSpiActual(data, byteCount);
}

// See See bus_pal.h for documentation on this method.
int BusPal::readSpiActual(uint8_t *data, size_t byteCount)
{
    assert(data);

    SpiWriteThenReadCommand command;
    command.command = kSpi_WriteThenRead;
    command.numBytesToWrite = 0;
    command.numBytesToRead = std::min<int>(byteCount, kBulkTransferMax);

    int actualBytes = buspal_serial_write(reinterpret_cast<uint8_t *>(&command), sizeof(command), true);
    if (actualBytes != sizeof(command))
    {
        Log::error("Error: Bus Pal read spi command failed\n");
        return 0;
    }

    uint8_t *respData = 0;

    while (respData == NULL)
    {
        respData = response(1);
    }

    if (respData[0] != kResponseOk)
    {
        Log::error("Error: bad response to spi read, response byte 0 = 0x%x\n", respData[0]);
        return 0;
    }

    int remainingBytes = byteCount;
    uint8_t *buffer = data;

    while (remainingBytes > 0)
    {
        actualBytes = buspal_serial_read(buffer, remainingBytes);
        if (actualBytes > 0)
        {
            remainingBytes -= actualBytes;
            buffer += actualBytes;
        }
        else
        {
            // If no bytes received, it is a timeout error.
            return 0;
        }
    }

    return byteCount;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::writeCan(const uint8_t *data, size_t byteCount)
{
    assert(data);

    CanWriteThenReadCommand command;
    command.command = kCan_WriteThenRead;
    command.numBytesToWrite = std::min<int>(byteCount, kBulkTransferMax);
    command.numBytesToRead = 0;

    m_canWriteByteCount += byteCount;

    int actualBytes = buspal_serial_write(reinterpret_cast<uint8_t *>(&command), sizeof(command), true);
    if (actualBytes != sizeof(command))
    {
        Log::error("Error: Bus Pal can write command failed\n");
        return false;
    }

    actualBytes = buspal_serial_write(const_cast<uint8_t *>(data), byteCount);
    if (actualBytes != byteCount)
    {
        Log::error("Error: Bus Pal can write command failed\n");
        return false;
    }

    uint8_t *respData = 0;
    uint8_t retry = 2;

    while (respData == NULL && (retry-- != 0))
    {
        respData = response(1);
    }

    if (respData == NULL || respData[0] != kResponseOk)
    {
        if (respData != NULL)
        {
            Log::error("Error: bad response to can write, response byte = 0x%x\n", respData[0]);
        }
        return false;
    }

    return true;
}

// See See BusPal.h for documentation on this method.
int BusPal::readCan(uint8_t *data, size_t byteCount)
{
    assert(data);

    if (m_canFirstReadDelay)
    {
#if defined(LINUX) || defined(MACOSX)
        sleep(m_canFirstReadDelay);
#else
        Sleep(m_canFirstReadDelay);
#endif
    }
    m_canFirstReadDelay = 0;

    CanWriteThenReadCommand command;
    command.command = kCan_WriteThenRead;
    command.numBytesToWrite = 0;
    command.numBytesToRead = std::min<int>(byteCount, kBulkTransferMax);

    int actualBytes = buspal_serial_write(reinterpret_cast<uint8_t *>(&command), sizeof(command), true);
    if (actualBytes != sizeof(command))
    {
        Log::error("Error: Bus Pal read can command failed\n");
        return 0;
    }

    uint8_t *respData = 0;

    while (respData == NULL)
    {
        respData = response(1);
    }

    if (respData[0] != kResponseOk)
    {
        Log::error("Error: bad response to can read, response byte 0 = 0x%x\n", respData[0]);
        return 0;
    }

    int remainingBytes = byteCount;
    uint8_t *buffer = data;

    while (remainingBytes > 0)
    {
        actualBytes = buspal_serial_read(buffer, remainingBytes);
        if (actualBytes > 0)
        {
            remainingBytes -= actualBytes;
            buffer += actualBytes;
        }
        else
        {
            // If no bytes received, it is a timeout error.
            return 0;
        }
    }

    return byteCount;
}

// See See bus_pal.h for documentation on this method.
bool BusPal::writeI2c(const uint8_t *data, size_t byteCount)
{
    assert(data);

    I2cWriteThenReadCommand command;
    command.command = kI2c_WriteThenRead;
    command.numBytesToWrite = std::min<int>(byteCount, kBulkTransferMax);
    command.numBytesToRead = 0;

    int actualBytes = buspal_serial_write(reinterpret_cast<uint8_t *>(&command), sizeof(command), true);
    if (actualBytes != sizeof(command))
    {
        Log::error("Error: Bus Pal i2c write command failed\n");
        return false;
    }

    actualBytes = buspal_serial_write(const_cast<uint8_t *>(data), byteCount);
    if (actualBytes != byteCount)
    {
        Log::error("Error: Bus Pal i2c write command failed\n");
        return false;
    }

    uint8_t *respData = NULL;
    while (respData == NULL)
    {
        respData = response(1);
    }

    if (respData[0] != kResponseOk)
    {
        Log::error("Error: bad response to i2c write, response byte = 0x%x\n", respData[0]);
        return false;
    }

    return true;
}

// See See bus_pal.h for documentation on this method.
int BusPal::readI2c(uint8_t *data, size_t byteCount)
{
    assert(data);

    I2cWriteThenReadCommand command;
    command.command = kI2c_WriteThenRead;
    command.numBytesToWrite = 0;
    command.numBytesToRead = std::min<int>(byteCount, kBulkTransferMax);

    int actualBytes = buspal_serial_write(reinterpret_cast<uint8_t *>(&command), sizeof(command), true);
    if (actualBytes != sizeof(command))
    {
        Log::error("Error: Bus Pal i2c read command failed\n");
        return 0;
    }

    uint8_t *respData = 0;

    while (respData == NULL)
    {
        respData = response(1);
    }

    if (respData[0] != kResponseOk)
    {
        Log::error("Error: bad response to i2c read, response byte 0 = 0x%x\n", respData[0]);
        return 0;
    }

    int remainingBytes = byteCount;
    uint8_t *buffer = data;

    while (remainingBytes > 0)
    {
        actualBytes = buspal_serial_read(buffer, remainingBytes);
        if (actualBytes > 0)
        {
            remainingBytes -= actualBytes;
            buffer += actualBytes;
        }
        else
        {
            // If no bytes received, it is a timeout error.
            return 0;
        }
    }

    return byteCount;
}

int BusPal::buspal_serial_read(uint8_t *buf, int size, bool isCommandData)
{
    int retVal = serial_read(m_fileDescriptor, (char *)buf, size);

    // If serial_read returned a 0 it is a timeout failure but doing this double
    // read fixes an intermittent issue when talking to bus pal the byte is actually sent from
    // the bus pal but serial_read returns empty however the byte is available on the next read
    if (retVal == 0)
    {
        retVal = serial_read(m_fileDescriptor, (char *)buf, size);
    }

    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes read in hex
        if (isCommandData)
        {
            Log::debug2("+");
        }
        else
        {
            Log::debug2("<");
        }

        for (int i = 0; i < retVal; i++)
        {
            Log::debug2("%02x", (uint8_t)buf[i]);
            if (i != (retVal - 1))
            {
                Log::debug2(" ");
            }
        }
        if (isCommandData)
        {
            Log::debug2("+\n");
        }
        else
        {
            Log::debug2(">\n");
        }
    }

    return retVal;
}

int BusPal::buspal_serial_write(uint8_t *buf, int size, bool isCommandData)
{
    if (Log::getLogger()->getFilterLevel() == Logger::kDebug2)
    {
        // Log bytes written in hex
        if (isCommandData)
        {
            Log::debug2("-");
        }
        else
        {
            Log::debug2("[");
        }

        for (int i = 0; i < size; i++)
        {
            Log::debug2("%02x", (uint8_t)buf[i]);
            if (i != (size - 1))
            {
                Log::debug2(" ");
            }
        }
        if (isCommandData)
        {
            Log::debug2("-\n");
        }
        else
        {
            Log::debug2("]\n");
        }
    }

    return serial_write(m_fileDescriptor, (char *)buf, size);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
