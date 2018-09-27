/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _Bootloader_h_
#define _Bootloader_h_

#include "Command.h"
#include "Packetizer.h"
#include "Peripheral.h"
#include "Logging.h"

#include <string.h>
#include <time.h>

//! @addtogroup blfwk
//! @{

namespace blfwk
{
/*!
 * @brief Represents the host bootloader.
 *
 * This class provides a convenient way to access other bootloader
 * framework objects.
 */
class Bootloader
{
public:
    //! @brief Default Constructor for Simulator.
    Bootloader();

    //! @brief Constructor.
    Bootloader(const Peripheral::PeripheralConfigData &config);

    //! @brief Destructor.
    virtual ~Bootloader();

    //! @brief Inject a command into the bootloader.
    //!
    //! @param cmd The command to send
    void inject(Command &cmd)
    {
        clock_t start = clock();
        cmd.sendTo(*m_hostPacketizer);
        clock_t finish = clock();
        Log::debug("  - took %2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
    }

    //! @brief Flush state.
    void flush();

    //! \brief Execute the execute command.
    void execute(uint32_t entry_point, uint32_t param = 0, uint32_t stack_pointer = 0);

    //! \brief Execute the reset command.
    void reset();

    //! \brief get Device's property by using get-property command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred while sending the
    //!                                 GetProperty(property) bootloader command.
    //!
    //! \param  property tag            The property tag
    //!
    //! \param  memoryIdorIndex         memoryId is required by GetProperty 25(External attribute)
    //!                                 Index is required by GetProperty 14/15(RAM start addr/RAM size)
    //!
    //! \return vector of the response values.
    uint32_vector_t getProperty(property_t tag, uint32_t memoryIdorIndex = kMemoryInternal);

    //! \brief Checks if Bootloader device supports a given command.
    //!
    //! \exception  std::runtime_error  Thrown if an error occurred while sending the
    //!                                 GetProperty(kProperty_AvailableCommands) bootloader command.
    //!
    //! \param  command The command to check.
    //!
    //! \return true if command is supported, false if not.
    bool isCommandSupported(const cmd_t &command);

    //! \brief Execute the get-property(current-version) command.
    standard_version_t getVersion();

    //! \brief Execute the get-property(flash-security-state) command.
    uint32_t getSecurityState();

    //! \brief Execute the get-property(max-supported-packet-size) command.
    uint32_t getDataPacketSize();

    //! \brief Send a ping if applicable.
    void ping(int retries, unsigned int delay, int comSpeed, int* actualComSpeed);

    //! @name Accessors.
    //@{

    //! @brief Get the host packetizer.
    Packetizer *getPacketizer() const { return m_hostPacketizer; }
    //@}

protected:
    Packetizer *m_hostPacketizer; //!< Packet interface to send commands on.
    FileLogger *m_logger;         //!< Singleton logger instance.
};

} // namespace blfwk

//! @}

#endif // _Bootloader_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
