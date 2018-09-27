/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BusPal_h_
#define _BusPal_h_

#include "host_types.h"

//! @addtogroup bus_pal
//! @{

namespace blfwk
{
/*!
 * @brief Interface with the BusPal.
 *
 * For hardware documentation, see http://dangerousprototypes.com/docs/Bus_Pirate.
 * This class is based on the pyBusPirateLite python package, see
 * http://dangerousprototypes.com/docs/Bus_Pirate_Scripting_in_Python.
 */
class BusPal
{
public:
    //! @brief Constants.
    enum
    {
        kResetCount = 20,               //!< Max number of nulls to send to enter BBIO mode
        kResponseOk = 0x01,             //!< Successful command response
        kBulkTransferMax = 4096,        //!< Max number of bytes per bulk transfer
        kMaxResponse = kBulkTransferMax //!< Max number of bytes in command response, including bulk transfer response,
                                        //!plus some padding
    };

    //! @brief BusPal Transports.
    enum bus_pal_function_t
    {
        kBusPalFunction_None,
        kBusPalFunction_SPI,
        kBusPalFunction_I2C,
        kBusPalFunction_CAN,
        kBusPalFunction_GPIO_CONFIG,
        kBusPalFunction_GPIO_SET,
        kBusPalFunction_FPGA_CLOCK_SET
    };

    //! @brief SPI clock polarity configuration.
    enum spi_clock_polarity_t
    {
        kSpiClockPolarity_ActiveHigh = 0, //!< Active-high SPI clock (idles low).
        kSpiClockPolarity_ActiveLow = 1   //!< Active-low SPI clock (idles high).
    };

    //! @brief SPI clock phase configuration.
    enum spi_clock_phase_t
    {
        kSpiClockPhase_FirstEdge =
            0, //!< First edge on SPSCK occurs at the middle of the first cycle of a data transfer.
        kSpiClockPhase_SecondEdge =
            1 //!< First edge on SPSCK occurs at the start of the first cycle of a data transfer.
    };

    //! @brief SPI data shifter direction options.
    enum spi_shift_direction_t
    {
        kSpiMsbFirst = 0, //!< Data transfers start with most significant bit.
        kSpiLsbFirst = 1  //!< Data transfers start with least significant bit.
    };

    //! @brief I2C default address.
    static const uint8_t kBusPalDefaultI2cSlaveAddress = 0x10; // I2C Slave 7-bit address

public:
    //! @brief BusPal configuration data.
    struct BusPalConfigData
    {
        bus_pal_function_t function;
        uint32_t spiSpeedKHz;
        spi_clock_polarity_t spiPolarity;
        spi_clock_phase_t spiPhase;
        spi_shift_direction_t spiDirection;
        uint8_t i2cAddress;
        uint32_t i2cSpeedKHz;
        uint32_t canSpeed;
        uint32_t canTxid;
        uint32_t canRxid;
        uint8_t gpioPort;
        uint8_t gpioPinNum;
        uint8_t gpioLevel;
        uint8_t gpioMux;
        uint32_t fpgaClockMhz;

        BusPalConfigData(bus_pal_function_t bus = BusPal::kBusPalFunction_None)
            : function(bus)
        {
            spiSpeedKHz = 100;
            spiPolarity = BusPal::kSpiClockPolarity_ActiveLow;
            spiPhase = BusPal::kSpiClockPhase_SecondEdge;
            spiDirection = BusPal::kSpiMsbFirst;
            i2cAddress = BusPal::kBusPalDefaultI2cSlaveAddress;
            i2cSpeedKHz = 100;
            canSpeed = 4; // 4: 1M
            canTxid = 0x321;
            canRxid = 0x123;
            gpioPort = 0;
            gpioPinNum = 0;
            gpioLevel = 0;
            gpioMux = 0;
            fpgaClockMhz = 0;
        }
    };

    //! @brief Constructor.
    BusPal(int fileDescriptor);

    //! @brief Destructor.
    virtual ~BusPal(){};

    //! @brief parse the passed in options into the config structure.
    static bool parse(const string_vector_t &params, BusPal::BusPalConfigData &config);

    //! @brief Reset to bit bang mode from another peripheral mode.
    virtual bool reset();

    //! @brief Reset the bus pal, comes up in terminal mode.
    virtual bool resetHardware();

    //! @brief Enter bit bang (binary scripting) mode.
    //!
    //! Call this first before entering other peripheral modes.
    virtual bool enterBitBangMode();

    //! @brief Enter Spi mode.
    virtual bool enterSpiMode();

    //! @brief Enter Can mode.
    virtual bool enterCanMode();

    //! @brief Enter I2c mode.
    virtual bool enterI2cMode();

    //! @brief Enter Uart mode. Not currently supported for bus pal
    virtual bool enterUartMode() { return false; }
    //! @brief Enter 1wire mode. Not currently supported for bus pal
    virtual bool enter1WireMode() { return false; }
    //! @brief Enter raw wire mode. Not currently supported for bus pal
    virtual bool enterRawWireMode() { return false; }
    //! @brief Raw configure pins.
    virtual bool rawConfigurePins(uint8_t port, uint8_t pin, uint8_t muxVal);

    //! @brief Set GPIO pin
    virtual bool rawSetPins(uint8_t port, uint8_t pin, uint8_t level);

    //! @brief Configure pins.
    virtual bool configurePins(uint8_t config = 0) { return true; }
    //! @brief Read pins.
    virtual uint8_t readPins() { return 0; }
    //! @brief Sets the FPGA clock, clock is in hertz
    virtual bool setFPGAClock(uint32_t clock);

    //! @brief Set SPI speed.
    virtual bool setSpiSpeed(unsigned int speed);

    //! @brief Sets the SPI configuration
    virtual bool setSpiConfig(spi_clock_polarity_t polarity, spi_clock_phase_t phase, spi_shift_direction_t direction);

    //! @brief Set I2c address
    virtual bool setI2cAddress(uint8_t address);

    //! @brief Set I2c speed
    virtual bool setI2cSpeed(uint32_t speed);

    //! @brief Set CAN speed.
    virtual bool setCanSpeed(unsigned int speed);

    //! @brief Set CAN txid.
    virtual bool setCanTxid(unsigned int txid);

    //! @brief Set CAN rxid.
    virtual bool setCanRxid(unsigned int rxid);

    //! @brief Read response.
    //!
    //! @retval NULL No response from device
    //! @retval Non-NULL Pointer to internal array of bytes at least size byteCount
    virtual uint8_t *response(size_t byteCount = 1);

    //! @brief writes the data over the previously selected interface
    virtual bool write(const uint8_t *data, size_t byteCount = 1);

    //! @brief Read data.
    virtual int read(uint8_t *data, size_t byteCount);

protected:
    //! @brief Write command, check string response.
    virtual bool writeCommand(uint8_t commandByte, const char *expectedResponse);

    //! @brief Write command, return 1 byte response.
    virtual uint8_t writeCommand(uint8_t commandByte);

    //! @brief write multi-byte command, return 1 byte response.
    virtual uint8_t writeCommand(uint8_t *command, unsigned int length);

    //! @brief write via Spi
    virtual bool writeSpi(const uint8_t *data, size_t byteCount = 1);

    //! @brief write via I2c
    virtual bool writeI2c(const uint8_t *data, size_t byteCount = 1);

    //! @brief write via Can
    virtual bool writeCan(const uint8_t *data, size_t byteCount = 1);

    //! @brief read via Spi
    virtual int readSpi(uint8_t *data, size_t byteCount);

    //! @brief read via Can
    virtual int readCan(uint8_t *data, size_t byteCount);

    //! @brief worker function to actually read SPI data
    int readSpiActual(uint8_t *data, size_t byteCount);

    //! @brief read via I2c
    virtual int readI2c(uint8_t *data, size_t byteCount);

    //! @brief Overriden serial_read for logging
    int buspal_serial_read(uint8_t *buf, int size, bool isCommandData = false);

    //! @brief Overriden serial_write for logging
    int buspal_serial_write(uint8_t *buf, int size, bool isCommandData = false);

    //! @brief Flushes the serial port of any RX data
    void flushRX();

protected:
    enum bus_pal_mode_t
    {
        kBusPalModeBitBang,
        kBusPalModeSpi,
        kBusPalModeI2c,
        kBusPalModeCan
    };
    int m_fileDescriptor;                   //!< PC COM port file descriptor.
    uint8_t m_responseBuffer[kMaxResponse]; //!< Command response buffer.
    bus_pal_mode_t m_mode;
    unsigned int m_spiWriteByteCount;
    unsigned int m_canWriteByteCount;
    unsigned int m_canFirstReadDelay;
};

} // namespace blfwk

//! @}

#endif // _BusPal_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
