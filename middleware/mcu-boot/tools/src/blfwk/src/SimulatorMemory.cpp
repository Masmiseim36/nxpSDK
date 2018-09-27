/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/Simulator.h"
#include "blfwk/SimulatorMemory.h"
#include "blfwk/Logging.h"
#include "memory/memory.h"
//#include "fsl_platform_status.h"
//#include <string.h>
#ifdef WIN32
#include <windows.h>
#elif LINUX
#include <linux/limits.h>
#include "sys/stat.h"
#include <string.h>
#else
#include "sys/stat.h"
#endif

using namespace blfwk;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Interface to simulator memory operations.
const memory_region_interface_t g_flashMemoryInterface = { NULL,           sim_flash_read, sim_flash_write,
                                                           sim_flash_fill, NULL,           NULL };

//! @brief Interface to simulator SRAM memory operations.
const memory_region_interface_t g_sramMemoryInterface = {
    NULL, sim_sram_mem_read, sim_sram_mem_write, sim_sram_mem_fill, NULL, NULL
};

//! @brief Interface to simulator device peripheral memory operations.
const memory_region_interface_t g_deviceMemoryInterface = {
    NULL, sim_device_mem_read, sim_device_mem_write, sim_device_mem_fill, NULL, NULL
};

//! @brief Memory map for KL25Z128.
memory_map_entry_t g_memoryMap[] = {
    { 0x00000000, 0x0001ffff, &g_flashMemoryInterface },  // Flash array
    { 0x1ffff000, 0x20002fff, &g_sramMemoryInterface },   // SRAM_L + SRAM_U
    { 0x40000000, 0x4007ffff, &g_deviceMemoryInterface }, // AIPS peripherals
    { 0x400ff000, 0x400fffff, &g_deviceMemoryInterface }, // GPIO
    { 0x44000000, 0x5fffffff, &g_deviceMemoryInterface }, // BME
    { 0xe0000000, 0xe00fffff, &g_deviceMemoryInterface }, // M0+ private peripherals
    { 0xf0003000, 0xf0003fff, &g_deviceMemoryInterface }, // MCM
    { 0xf8000000, 0xffffffff, &g_deviceMemoryInterface }, // IOPORT (single-cycle GPIO)
    { 0 }                                                 // Terminator
};

#ifdef WIN32
//! @brief File separator character.
static const string k_fileSeparator("\\");
static const unsigned k_maxPath = MAX_PATH;
#else
static const string k_fileSeparator("/");
static const unsigned k_maxPath = PATH_MAX;
#endif

//! @brief Options state file name.
static const string k_optionsFileName = "state_init.dat";

//! @brief Memory state file name.
static const string k_memoryFileName = "state_mem0.dat";

// Forward function declarations
static status_t sim_mem_read(int mapIndex, uint32_t address, uint32_t length, uint8_t *buffer);
static status_t sim_mem_write(int mapIndex, uint32_t address, uint32_t length, const uint8_t *buffer);
static status_t sim_mem_fill(int mapIndex, uint32_t address, uint32_t length, uint32_t pattern);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//@brief Create all directories in a path if they don't exist.
//!
//! Errors are ignored.
//!
//! @param path Directory path, must end with a separator.
static void createPath(const char *path)
{
    char folder[k_maxPath] = { 0 };
    const char *end = strchr(path, k_fileSeparator[0]);

    while (end != NULL)
    {
        strncpy(folder, path, end - path + 1);
#ifdef WIN32
        CreateDirectory(folder, NULL);
#else
        mkdir(folder, 0755);
#endif
        end = strchr(++end, k_fileSeparator[0]);
    }
}

// See host_memory.h for documentation of this method.
bool MemoryStore::open(const string &pathToDir, bool forceCreate)
{
    // Create all directories in the path if they don't exist.
    // Errors will be caught by trying to open the file below.
    string path(pathToDir);
    path += k_fileSeparator;
    createPath(path.c_str());

    // Create the file name, replacing the '0' with the actual map index.
    path += k_memoryFileName;
    size_t pos = path.find_last_of('0');
    path.replace(pos, 1, 1, static_cast<char>(m_mapIndex + '0'));

    close();
    m_memoryFile = NULL;
    bool doFill = false;

    if (!forceCreate)
    {
        // Try to open existing file.
        m_memoryFile = fopen(path.c_str(), "r+b");
    }

    if (!m_memoryFile)
    {
        // Create new file.
        m_memoryFile = fopen(path.c_str(), "w+b");
        doFill = true;
    }

    if (m_memoryFile)
    {
        m_startAddress = g_memoryMap[m_mapIndex].startAddress;
        m_size = (g_memoryMap[m_mapIndex].endAddress + 1) - m_startAddress;
        if (doFill)
        {
            erase();
        }
    }
    else
    {
        Log::error("Error: cannot open or create memory file '%s'\n", path.c_str());
        return false;
    }

    return true;
}

// See host_memory.h for documentation of this method.
size_t MemoryStore::read(long int offset, size_t size, unsigned char *buffer)
{
    if (m_memoryFile)
    {
        fseek(m_memoryFile, offset - m_startAddress, SEEK_SET);
        return fread(buffer, 1, size, m_memoryFile);
    }
    return 0;
}

// See host_memory.h for documentation of this method.
size_t MemoryStore::write(long int offset, size_t size, const unsigned char *buffer)
{
    if (m_memoryFile)
    {
        fseek(m_memoryFile, offset - m_startAddress, SEEK_SET);
        return fwrite(buffer, 1, size, m_memoryFile);
    }
    return 0;
}

// See host_memory.h for documentation of this method.
void MemoryStore::erase(long int offset, size_t size)
{
    if (m_memoryFile)
    {
        unsigned char *fill = new unsigned char[size];
        memset(fill, m_fillByte, size);
        fseek(m_memoryFile, offset - m_startAddress, SEEK_SET);
        fwrite(fill, 1, size, m_memoryFile);
        delete[] fill;
    }
}

//! @brief Read from simulated memory.
//!
//! @param mapIndex Index into memory map array
//! @param address Simulated address
//! @param length Number of bytes to read
//! @param buffer Destination buffer
//!
//! @retval kStatusMemoryReadFailed
//! @retval kStatus_Success
static status_t sim_mem_read(int mapIndex, uint32_t address, uint32_t length, uint8_t *buffer)
{
    Simulator &sim = Simulator::getSimulator();
    MemoryStore *mem = sim.getMemoryStore(mapIndex);
    if (mem->read(address, length, buffer) != length)
    {
        return kStatusMemoryReadFailed;
    }
    else
    {
        return kStatus_Success;
    }
}

//! @brief Write to simulated memory.
//!
//! @param mapIndex Index into memory map array
//! @param address Simulated address
//! @param length Number of bytes to write
//! @param buffer Source buffer
//!
//! @retval kStatusMemoryWriteFailed
//! @retval kStatus_Success
static status_t sim_mem_write(int mapIndex, uint32_t address, uint32_t length, const uint8_t *buffer)
{
    Simulator &sim = Simulator::getSimulator();
    MemoryStore *mem = sim.getMemoryStore(mapIndex);
    if (mem->write(address, length, buffer) != length)
    {
        return kStatusMemoryWriteFailed;
    }
    else
    {
        return kStatus_Success;
    }
}

//! @brief Fill simulated memory.
//!
//! Pattern is a 4-byte pattern that can be written at any byte alignment.
//! Pattern must contain all bytes to write. To fill every byte
//! with the same value, the value must be repeat in the word.
//! For example, pattern=0x55555555.
//!
//! @param mapIndex Index into memory map array
//! @param address Simulated address
//! @param length Number of bytes to write
//! @param pattern 4-byte pattern to write
//!
//! @retval kStatusMemoryWriteFailed
//! @retval kStatus_Success
static status_t sim_mem_fill(int mapIndex, uint32_t address, uint32_t length, uint32_t pattern)
{
    Simulator &sim = Simulator::getSimulator();
    MemoryStore *mem = sim.getMemoryStore(mapIndex);
    unsigned char b3 = (unsigned char)((pattern >> 24) & 0xff);
    unsigned char b2 = (unsigned char)((pattern >> 16) & 0xff);
    unsigned char b1 = (unsigned char)((pattern >> 8) & 0xff);
    unsigned char b0 = (unsigned char)(pattern & 0xff);
    while (length)
    {
        if (mem->write(address++, 1, &b0) != 1)
        {
            Log::error("Error: memory write failed\n");
            return kStatusMemoryWriteFailed;
        }
        if (--length == 0)
            break;

        if (mem->write(address++, 1, &b1) != 1)
        {
            Log::error("Error: memory write failed\n");
            return kStatusMemoryWriteFailed;
        }
        if (--length == 0)
            break;

        if (mem->write(address++, 1, &b2) != 1)
        {
            Log::error("Error: memory write failed\n");
            return kStatusMemoryWriteFailed;
        }
        if (--length == 0)
            break;

        if (mem->write(address++, 1, &b3) != 1)
        {
            Log::error("Error: memory write failed\n");
            return kStatusMemoryWriteFailed;
        }
        --length;
    }

    return kStatus_Success;
}

// See host_memory.h for documentation of this method.
status_t sim_flash_read(uint32_t address, uint32_t length, uint8_t *buffer)
{
    return sim_mem_read(MemoryStore::kMapIndexFlash, address, length, buffer);
}

// See host_memory.h for documentation of this method.
status_t sim_flash_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    return sim_mem_write(MemoryStore::kMapIndexFlash, address, length, buffer);
}

// See host_memory.h for documentation of this method.
status_t sim_flash_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    return sim_mem_fill(MemoryStore::kMapIndexFlash, address, length, pattern);
}

// See host_memory.h for documentation of this method.
status_t sim_flash_erase(uint32_t address, uint32_t length)
{
    host_flash_erase_region(address, length);
    return kStatus_Success;
}

// See host_memory.h for documentation of this method.
status_t sim_flash_erase_all(void)
{
    host_flash_erase_all();
    return kStatus_Success;
}

// See host_memory.h for documentation of this method.
status_t sim_sram_mem_read(uint32_t address, uint32_t length, uint8_t *buffer)
{
    return sim_mem_read(MemoryStore::kMapIndexSRAM, address, length, buffer);
}

// See host_memory.h for documentation of this method.
status_t sim_sram_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    return sim_mem_write(MemoryStore::kMapIndexSRAM, address, length, buffer);
}

// See host_memory.h for documentation of this method.
status_t sim_sram_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    return sim_mem_fill(MemoryStore::kMapIndexSRAM, address, length, pattern);
}

// See host_memory.h for documentation of this method.
status_t sim_device_mem_read(uint32_t address, uint32_t length, uint8_t *buffer)
{
    Log::warning("Warning: device peripheral memory not simulated\n");
    return kStatus_Success;
}

// See host_memory.h for documentation of this method.
status_t sim_device_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer)
{
    Log::warning("Warning: device peripheral memory not simulated\n");
    return kStatus_Success;
}

// See host_memory.h for documentation of this method.
status_t sim_device_mem_fill(uint32_t address, uint32_t length, uint32_t pattern)
{
    Log::warning("Warning: device peripheral memory not simulated\n");
    return kStatus_Success;
}

// See memory.h for documentation of this method.
void host_flash_erase_all()
{
    Simulator &sim = Simulator::getSimulator();
    MemoryStore *mem = sim.getMemoryStore(MemoryStore::kMapIndexFlash);
    mem->erase();
}

// See memory.h for documentation of this method.
void host_flash_erase_all_unsecure()
{
    host_flash_erase_all();
}

// See memory.h for documentation of this method.
void host_flash_erase_region(uint32_t address, uint32_t count)
{
    Simulator &sim = Simulator::getSimulator();
    MemoryStore *mem = sim.getMemoryStore(MemoryStore::kMapIndexFlash);
    mem->erase(address, count);
}

////////////////////////////////////////////////////////////////////////////////
// Options Store operations
////////////////////////////////////////////////////////////////////////////////

// See host_memory.h for documentation of this method.
bool OptionsStore::init(const string &pathToDir, bool forceCreate)
{
    m_optionsFile.assign(pathToDir);
    m_optionsFile += k_fileSeparator + k_optionsFileName;

    // Do nothing if forceCreate requested - file will be created by persist().
    if (!forceCreate)
    {
        FILE *options = fopen(m_optionsFile.c_str(), "r");
        if (options)
        {
            // Read property store values.
            fread(g_bootloaderContext.propertyInterface->store, sizeof(*(g_bootloaderContext.propertyInterface->store)),
                  1, options);

            fclose(options);
        }
    }

    return true;
}

// See host_memory.h for documentation of this method.
void OptionsStore::persist()
{
    FILE *options = fopen(m_optionsFile.c_str(), "w");
    if (!options)
    {
        Log::error("Error: cannot create options file '%s'\n", m_optionsFile.c_str());
        return;
    }

    // Write property store values.
    fwrite(g_bootloaderContext.propertyInterface->store, sizeof(*(g_bootloaderContext.propertyInterface->store)), 1,
           options);

    fclose(options);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
