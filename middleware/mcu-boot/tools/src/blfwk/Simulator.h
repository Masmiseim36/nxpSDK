/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _Simulator_h_
#define _Simulator_h_

#include "Bootloader.h"
#include "Command.h"
#include "SimulatorMemory.h"
#include "SimPacketizer.h"
#include "Peripheral.h"
#include "Logging.h"

#include "bootloader/bl_command.h"
#include "bootloader/bl_context.h"

namespace blfwk
{
/*!
 * @brief Represents the host bootloader.
 *
 * This class provides a convenient way to access other bootloader
 * framework objects.
 */
class Simulator : public Bootloader
{
public:
    //! @brief A vector of memory stores.
    typedef std::vector<blfwk::MemoryStore *> memory_vector_t;

    //! @brief Get the singleton simulator object.
    static Simulator &getSimulator()
    {
        //! @brief Singleton object.
        static Simulator theSimulator;
        return theSimulator;
    }

    //! @brief Destructor.
    virtual ~Simulator();

    //! @brief Initialize.
    void init();

    //! @brief Configure and open state files.
    //!
    //! Must be called to open or create state files.
    //!
    //! @param pathToDir Directory for state files.
    //! @param forceCreate True to re-create state files even if they exist.
    bool openStateFiles(const std::string &pathToDir, bool forceCreate);

    //! @name Accessors.
    //@{

    //! @brief Get the host packetizer.
    SimPacketizer *getHost() const { return dynamic_cast<SimPacketizer *>(m_hostPacketizer); }
    //! @brief Get the device packetizer.
    SimPacketizer *getDevice() const { return m_devicePacketizer; }
    //! @brief Get a device state memory store.
    //!
    //! index Index into memory map for the simulated device.
    MemoryStore *getMemoryStore(int index) const { return m_memoryStore[index]; }
    //@}

protected:
    //! @brief Constructor.
    Simulator();
    SimPacketizer *m_devicePacketizer; //!< Packet interface to recieve commands on.
    uchar_deque_t m_commandStream;
    uchar_deque_t m_responseStream;
    command_processor_data_t m_commandProcessor;
    bool m_areStateFilesOpen;      //!< True if state files are in use
    memory_vector_t m_memoryStore; //!< Vector of memory stores, one per map entry.
    OptionsStore m_optionsStore;   //!< Persistent options store.
    property_store_t m_propertyStore;
    peripheral_descriptor_t m_activePeripheral; //!< Descriptor for the active peripheral.
    FileLogger *m_logger;                       //!< Singleton logger instance.
};

} // namespace blfwk

#endif // _Simulator_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
