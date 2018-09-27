/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwkdll.h"
#include "blfwk/Bootloader.h"
#include "blfwk/Updater.h"
#include "blfwk/SerialPacketizer.h"
#include "blfwk/Crc.h"

namespace blfwkdll
{
// See blfwkdll.h for documentation of this method.
Updater::Updater(PeripheralConfig *config)
{
    blfwk::Peripheral::PeripheralConfigData *pinnedConfig = (blfwk::Peripheral::PeripheralConfigData *)config;
    updater = NULL;
    try
    {
        updater = new blfwk::Updater(*pinnedConfig);
    }
    catch (exception e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
Updater::~Updater()
{
    // Clean up unmanaged code.
    if (updater != NULL)
    {
        delete (blfwk::Updater *)updater;
        updater = NULL;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::registerCallback(void (*progress)(int, int, int), bool *abort)
{
    ((blfwk::Updater *)updater)->registerCallback(progress, abort);
}

// See blfwkdll.h for documentation of this method.
uint32_t Updater::flashFirmware(std::string *filename, uint32_t baseAddress, uint32_t memoryId)
{
    try
    {
        uint32_t status;
        status = ((blfwk::Updater *)updater)->flashFirmware(filename->c_str(), baseAddress, memoryId);
        return status;
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::eraseAllUnsecure()
{
    try
    {
        ((blfwk::Updater *)updater)->eraseAllUnsecure();
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::eraseRegion(uint32_t start, uint32_t length, uint32_t memoryId)
{
    try
    {
        ((blfwk::Updater *)updater)->eraseRegion(start, length, memoryId);
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::eraseAll(uint32_t memoryId)
{
    try
    {
        ((blfwk::Updater *)updater)->eraseAll(memoryId);
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::unlockWithKey(const std::string &backdoorKey)
{
    try
    {
        ((blfwk::Updater *)updater)->unlock(backdoorKey);
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

/* clang-format off */
/* the for each statement is non-standard and not supported by clang format */
// See blfwkdll.h for documentation of this method.
bool Updater::isCommandSupported(const std::string &command)
{
    try
    {
        for each(blfwk::cmd_t var in blfwk::kCommands)
            {
                if (command.compare(var.name) == 0)
                {
                    return ((blfwk::Updater *)updater)->isCommandSupported(var);
                }
            }
        return false;
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}
/* clang-format on */

// See blfwkdll.h for documentation of this method.
void Updater::getProperty(PROPERTY_TAG tag, uint32_t *responseWords)
{
    try
    {
        blfwk::property_t propertytag = blfwk::kProperties.at(tag);
        std::vector<uint32_t> value = ((blfwk::Updater *)updater)->getProperty(propertytag);
        for (size_t i = 0; i < value.size(); i++)
        {
            responseWords[i] = value.at(i);
        }
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
uint32_t Updater::getInternalFlashSize(void)
{
    try
    {
        return ((blfwk::Updater *)updater)->getInternalFlashSize();
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
uint32_t Updater::getInternalRAMSize(void)
{
    try
    {
        return ((blfwk::Updater *)updater)->getInternalRAMSize();
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::programOnce(uint32_t index, uint32_t byteCount, const std::string &data, bool isLsb)
{
    try
    {
        ((blfwk::Updater *)updater)->programOnce(index, byteCount, data, isLsb);
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::reset()
{
    try
    {
        ((blfwk::Updater *)updater)->reset();
        // delete updater;
        // updater = NULL;
    }
    catch (const std::exception &e)
    {
        // delete this;
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
uint32_t Updater::getSerialProtocolVersion()
{
    try
    {
        return ((blfwk::Updater *)updater)->getPacketizer()->getVersion().version;
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::ping(int32_t retries, uint32_t delay, uint32_t comSpeed)
{
    static uint32_t actualComSpeed = 0;
    uint32_t xferComSpeed = (actualComSpeed == 0) ? comSpeed : actualComSpeed;
    try
    {
        ((blfwk::Updater *)updater)->ping(retries, delay, xferComSpeed, (int *)&actualComSpeed);
    }
    catch (const std::exception &e)
    {
        actualComSpeed = comSpeed;
        throw e;
    }
}

// See blfwkdll.h for documentation of this method.
void Updater::getSupportedMemories(MemoryList &list)
{
    list.push_back(make_pair(kMemory_Internal.memoryId, kMemory_Internal.description));
    list.push_back(make_pair(kMemory_ExecuteOnly.memoryId, kMemory_ExecuteOnly.description));
    for (MemoryArray::const_iterator it = kMemories.begin(); it != kMemories.end(); it++)
    {
        list.push_back(make_pair(it->memoryId, it->description));
    }
}

// See blfwkdll.h for documentation of this method.
bool Updater::IsMemorySpaceId(uint32_t memoryId)
{
    if ((GROUPID(memoryId) != kGroup_Internal) || (memoryId == kMemoryInternal))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// See blfwkdll.h for documentation of this method.
uint32_t Crc::calculate_application_crc32(const uint8_t *start, uint32_t length)
{
    return blfwk::Crc::calculate_application_crc32(start, length);
}

} // namespace blfwkdll;
