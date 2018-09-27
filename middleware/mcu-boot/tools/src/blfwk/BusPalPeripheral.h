/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BusPalPeripheral_h_
#define _BusPalPeripheral_h_

#include "UartPeripheral.h"
#include "BusPal.h"

//! @addtogroup bus_pal_peripheral
//! @{

namespace blfwk
{
/*!
 * @brief Peripheral that talks to the target device over BusPal UART hardware.
 */
class BusPalUartPeripheral : public UartPeripheral
{
public:
    //! @brief Parameterized constructor that opens the serial port.
    //!
    //! Opens and configures the port. Throws exception if port configuration fails.
    //!
    //! Note: following COM port configuration is assumed: 8 bits, 1 stop bit, no parity.
    //!
    //! @param port OS file path for COM port. For example "COM1" on Windows.
    //! @param speed Port speed, e.g. 9600.
    BusPalUartPeripheral(const char *port, long speed, const BusPal::BusPalConfigData &config);

    //! @brief Destructor.
    virtual ~BusPalUartPeripheral();

    //! @brief configure the bus pal with the passed in options
    void configure(const BusPal::BusPalConfigData &config);

    //! @brief Read bytes.
    //!
    //! @param buffer Pointer to buffer
    //! @param requestedBytes Number of bytes to read
    virtual status_t read(uint8_t *buffer, uint32_t requestedBytes, uint32_t *actualBytes, uint32_t timeoutMs);

    //! @brief Write bytes.
    //!
    //! @param buffer Pointer to buffer to write
    //! @param byteCount Number of bytes to write
    virtual status_t write(const uint8_t *buffer, uint32_t byteCount);

    virtual _host_peripheral_types get_type(void) { return kHostPeripheralType_BUSPAL_UART; }

protected:
    BusPal *m_busPal; //!< Represents Bus Pal hardware.
};

} // namespace blfwk

//! @}

#endif // _BusPalPeripheral_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
