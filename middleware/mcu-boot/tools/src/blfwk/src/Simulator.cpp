/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/Simulator.h"

#include "bootloader/bl_version.h"
#include "memory/memory.h"
#include "packet/serial_packet.h"
#include "property/property.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace blfwk;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See host_peripheral.h for documentation of this method.
status_t host_peripheral_read(const peripheral_descriptor_t *self, uint8_t *buffer, uint32_t requestedBytes)
{
    Simulator &sim = Simulator::getSimulator();
    Peripheral *device = sim.getHost()->getPeripheral();
    return device->read(buffer, requestedBytes, NULL, 0);
}

// See host_peripheral.h for documentation of this method.
status_t host_peripheral_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount)
{
    Simulator &sim = Simulator::getSimulator();
    Peripheral *device = sim.getHost()->getPeripheral();
    device->write(buffer, byteCount);
    return kStatus_Success;
}

// See host_peripheral.h for documentation of this method.
status_t device_peripheral_read(const peripheral_descriptor_t *self, uint8_t *buffer, uint32_t requestedBytes)
{
    Simulator &sim = Simulator::getSimulator();
    Peripheral *device = sim.getDevice()->getPeripheral();
    device->read(buffer, requestedBytes, NULL, 0);
    return kStatus_Success;
}

// See host_peripheral.h for documentation of this method.
status_t device_peripheral_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount)
{
    Simulator &sim = Simulator::getSimulator();
    Peripheral *device = sim.getDevice()->getPeripheral();
    device->write(buffer, byteCount);
    return kStatus_Success;
}

// See host_packetizer.h for documentation of this method.
status_t host_packet_read(const peripheral_descriptor_t *self,
                          uint8_t **packet,
                          uint32_t *packetLength,
                          packet_type_t packetType)
{
    Simulator &sim = Simulator::getSimulator();
    Packetizer *device = sim.getDevice();
    return device->readPacket(packet, packetLength, packetType);
}

// See host_packetizer.h for documentation of this method.
status_t host_packet_write(const peripheral_descriptor_t *self,
                           const uint8_t *packet,
                           uint32_t byteCount,
                           packet_type_t packetType)
{
    Simulator &sim = Simulator::getSimulator();
    Packetizer *device = sim.getDevice();
    return device->writePacket(packet, byteCount, packetType);
}

// See host_packetizer.h for documentation of this method.
void host_packet_abort(const peripheral_descriptor_t *self)
{
    Simulator &sim = Simulator::getSimulator();
    Packetizer *host = sim.getHost();
    host->setAborted(true);
}

// See host_packetizer.h for documentation of this method.
status_t host_packet_finalize(const peripheral_descriptor_t *self)
{
    Simulator &sim = Simulator::getSimulator();
    Packetizer *host = sim.getHost();
    host->finalize();
    return kStatus_Success;
}

uint32_t host_get_max_packet_size(const peripheral_descriptor_t *self)
{
    Simulator &sim = Simulator::getSimulator();
    Packetizer *host = sim.getHost();
    return host->getMaxPacketSize();
}

void host_delay(uint32_t milliseconds)
{
// @todo implement for non-win32
#ifdef WIN32
    Sleep(milliseconds);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Bootloader global context data.
//!
//! Referenced by both the simulated host and simulated device sides.
bootloader_context_t g_bootloaderContext = {
    &g_memoryInterface,   // Memory interface.
    g_memoryMap,          // Memory map.
    &g_propertyInterface, // Property store interface.
    &g_commandInterface,  // Command processor interface.
    NULL,                 // Flash driver interface - not used on host
    NULL,                 // Peripheral array - filled in at run-time.
    NULL,                 // Active peripheral - filled in at run-time.
    0                     // Flash driver state - typed to a u32 and unused for host
};

//! @brief Interface to host peripheral operations.
const peripheral_byte_inteface_t g_hostPeripheralInterface = {
    NULL,                 // init
    host_peripheral_read, // read
    host_peripheral_write // write
};

//! @brief Interface to device peripheral operations.
const peripheral_byte_inteface_t g_devicePeripheralInterface = {
    NULL,                   // init
    device_peripheral_read, // read
    device_peripheral_write // write
};

//! @brief Interface to host packet operations.
const peripheral_packet_interface_t g_hostPacketInterface = {
    NULL,                     // init
    host_packet_read,         // readPacket
    host_packet_write,        // writePacket
    host_packet_abort,        // abortPacket
    host_packet_finalize,     // finalize
    host_get_max_packet_size, // getMaxPacketSize
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
// See host_bootloader.h for documentation of this method.
Simulator::Simulator()
    : Bootloader()
    , m_devicePacketizer(NULL)
{
    // Create host peripheral and packetizer.
    SimPeripheral *hostPeripheral = new SimPeripheral(&m_responseStream, &m_commandStream);
    m_hostPacketizer = new SimPacketizer(hostPeripheral);

    // Enable the host packetizer to pump the simulator state machine as necessary.
    ((SimPacketizer *)m_hostPacketizer)->enableSimulatorPump();

    // Create device peripheral and packetizer.
    SimPeripheral *devicePeripheral = new SimPeripheral(&m_commandStream, &m_responseStream);
    m_devicePacketizer = new SimPacketizer(devicePeripheral);

    init();
}

// See host_bootloader.h for documentation of this method.
Simulator::~Simulator()
{
    // Delete packetizer should close handles and free memory on Peripheral.
    delete m_devicePacketizer;

    // Delete all memory stores that were new'd in init().
    m_memoryStore.clear();
}

// See host_bootloader.h for documentation of this method.
void Simulator::init()
{
    // Initialize the property store component.
    bootloader_property_init();

    g_bootloaderContext.activePeripheral = &m_activePeripheral;

    // Create all memory stores.
    // Stores are never deleted because bootloader is a singleton.
    FlashMemoryStore *flash = new FlashMemoryStore;
    m_memoryStore.push_back(flash);
    SramMemoryStore *sram = new SramMemoryStore;
    m_memoryStore.push_back(sram);

    // Initialize the command processor component.
    bootloader_command_init();
    m_commandProcessor.state = kCommandState_CommandPhase;

    // Use the host peripheral and packet interfaces.
    m_activePeripheral.byteInterface = &g_hostPeripheralInterface;
    m_activePeripheral.packetInterface = &g_hostPacketInterface;
}

// See host_bootloader.h for documentation of this method.
bool Simulator::openStateFiles(const string &pathToDir, bool forceCreate)
{
    // Open all memory stores.
    memory_vector_t::iterator it = m_memoryStore.begin();
    for (; it != m_memoryStore.end(); ++it)
    {
        if (!(*it)->open(pathToDir, forceCreate))
        {
            return false;
        }
    }

    if (!m_optionsStore.init(pathToDir, forceCreate))
    {
        return false;
    }

    m_areStateFilesOpen = true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
