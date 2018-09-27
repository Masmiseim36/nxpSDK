/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/Updater.h"

using namespace blfwk;

Updater::Updater(const Peripheral::PeripheralConfigData &config)
    : Bootloader(config)
    , m_base_address(0)
    , m_sourceFile(NULL)
    , m_operation(kUpdaterOperation_Update)
    , m_progressCallback(NULL)
    , m_progress()
{
    m_version = getVersion();
}

Updater::~Updater()
{
}

// See Updater.h for documentation of this method.
status_t Updater::flashFirmware(const char *filename, uint32_t base_address, uint32_t memoryId)
{
    m_base_address = base_address;
    m_memoryId = memoryId;
    // Create the SourceFile
    m_sourceFile = SourceFile::openFile(filename);

    // Initialize the Operation structure.
    m_operation.tasks.push_back(updater_task_t(kUpdaterTask_Erasing, m_sourceFile->getSize()));
    m_operation.tasks.push_back(updater_task_t(kUpdaterTask_Flashing, m_sourceFile->getSize()));
    m_operation.current_task = 0;

    if (m_sourceFile->getFileType() == SourceFile::source_file_t::kSBSourceFile)
    {
        return flashFromSBFile(filename);
    }
    else if (m_sourceFile->getFileType() == SourceFile::source_file_t::kELFSourceFile)
    {
        throw std::runtime_error("ELF files are not yet supported.");
    }
    else
    {
        return flashFromSourceFile();
    }
}

// See Updater.h for documentation of this method.
void Updater::eraseAllUnsecure()
{
    if (!isCommandSupported(kCommand_FlashEraseAllUnsecure))
    {
        throw std::runtime_error("FlashEraseAllUnsecure isn't supported!");
    }

    FlashEraseAllUnsecure cmd;
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if (fw_status != kStatus_Success)
    {
        throw std::runtime_error(fw_msg);
    }
}

// See Updater.h for documentation of this method.
void Updater::eraseRegion(uint32_t start, uint32_t length, uint32_t memoryId)
{
    if (!isMemorySupported(memoryId))
    {
        throw std::runtime_error("Selected memory isn't supported!");
    }
    m_memoryId = memoryId;
    return eraseFlashRegion(start, length);
}

void Updater::eraseAll(uint32_t memoryId)
{
    if (!isMemorySupported(memoryId))
    {
        throw std::runtime_error("Selected memory isn't supported!");
    }
    m_memoryId = memoryId;
    return eraseFlashAll();
}

// See Updater.h for documentation of this method.
void Updater::unlock(string backdoor_key)
{
    if (!isCommandSupported(kCommand_FlashSecurityDisable))
    {
        throw std::runtime_error("FlashSecurityDisable isn't supported!");
    }
    if (backdoor_key.length() != 16)
    {
        throw std::runtime_error("Illegal BackdoorKey");
    }

    FlashSecurityDisable cmd(strtoul(backdoor_key.substr(8, 8).c_str(), NULL, 16),
                             strtoul(backdoor_key.substr(0, 8).c_str(), NULL, 16));
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if (fw_status != kStatus_Success)
    {
        throw std::runtime_error(fw_msg);
    }
}

uint32_t Updater::getInternalFlashSize(void)
{
    uint32_t totalSize = 0;

    uint32_t firstStartAddress = 0;
    uint32_t fw_status;
    for (uint32_t index = 0;; index++)
    {
        // Get internal on-chip Flash start address.
        GetProperty startAddr(kProperty_FlashStartAddress, index);
        inject(startAddr);
        fw_status = startAddr.getResponseValues()->at(0);
        if (fw_status == kStatus_UnknownProperty)
        {
            // UnknownProperty means that there is no internal flash on current device.
            return 0;
        }
        else if (fw_status == kStatus_InvalidArgument)
        {
            break;
        }
        else if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            throw std::runtime_error(startAddr.getStatusMessage(fw_status));
        }

        if (index == 0)
        {
            firstStartAddress = startAddr.getResponseValues()->at(1);
        }
        else if (firstStartAddress == startAddr.getResponseValues()->at(1))
        {
            // If a Flash region's start address is the same as the first region.
            // That means all Flash regions are listed.
            // Then break the for-loop.
            break;
        }

        GetProperty size(kProperty_FlashSizeInBytes, index);
        inject(size);
        fw_status = size.getResponseValues()->at(0);
        if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            throw std::runtime_error(size.getStatusMessage(fw_status));
        }

        totalSize += size.getResponseValues()->at(1);
    }

    return totalSize;
}

uint32_t Updater::getInternalRAMSize(void)
{
    uint32_t totalSize = 0;

    uint32_t firstStartAddress = 0;
    uint32_t fw_status;
    for (uint32_t index = 0;; index++)
    {
        // Get internal on-chip RAM start address.
        GetProperty startAddr(kProperty_RAMStartAddress, index);
        inject(startAddr);
        fw_status = startAddr.getResponseValues()->at(0);
        if (fw_status == kStatus_UnknownProperty)
        {
            // UnknownProperty means that there is no internal flash on current device.
            return 0;
        }
        else if (fw_status == kStatus_InvalidArgument)
        {
            break;
        }
        else if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            throw std::runtime_error(startAddr.getStatusMessage(fw_status));
        }

        if (index == 0)
        {
            firstStartAddress = startAddr.getResponseValues()->at(1);
        }
        else if (firstStartAddress == startAddr.getResponseValues()->at(1))
        {
            // If a RAM region's start address is the same as the first region.
            // That means all RAM regions are listed.
            // Then break the for-loop.
            break;
        }

        GetProperty size(kProperty_RAMSizeInBytes, index);
        inject(size);
        fw_status = size.getResponseValues()->at(0);
        if (fw_status != kStatus_Success)
        {
            // Failed to communicate with the device.
            throw std::runtime_error(size.getStatusMessage(fw_status));
        }

        totalSize += size.getResponseValues()->at(1);
    }

    return totalSize;
}

// See Updater.h for documentation of this method.
void Updater::programOnce(uint32_t index, uint32_t byteCount, string data, bool isLsb)
{
    if (!isCommandSupported(kCommand_FlashProgramOnce))
    {
        throw std::runtime_error("FlashProgramOnce isn't supported!");
    }
    if ((byteCount != 4) && (byteCount != 8))
    {
        throw std::runtime_error("Illegal byteCount! Must be 4 or 8");
    }
    if ((data.length() != 8) && (data.length() != 16))
    {
        throw std::runtime_error("Illegal data length. Must hex digits with no leading 0x");
    }

    FlashProgramOnce cmd(index, byteCount, strtoul(data.substr(8, 8).c_str(), NULL, 16),
                         strtoul(data.substr(0, 8).c_str(), NULL, 16), isLsb);
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if (fw_status != kStatus_Success)
    {
        throw std::runtime_error(fw_msg);
    }
}

// See Updater.h for documentation of this method.
void Updater::eraseFlashRegion(uint32_t start, uint32_t length)
{
    // Align the address and length to 1K boundary.
    uint32_t alignedStart = start & (~(MinEraseAlignment - 1));
    uint32_t alignedLength = ((start + length + MinEraseAlignment) & (~(MinEraseAlignment - 1))) - alignedStart;

    // Inject the flash-erase-region(start, length) command.
    FlashEraseRegion cmd(alignedStart, alignedLength, m_memoryId);
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if (fw_status != kStatus_Success)
    {
        throw std::runtime_error(fw_msg);
    }
}

// See Updater.h for documentation of this method.
void Updater::eraseFlashAll()
{
    // Inject the flash-erase-all command.
    FlashEraseAll cmd(m_memoryId);
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if (fw_status != kStatus_Success)
    {
        throw std::runtime_error(fw_msg);
    }
}

// See Updater.h for documentation of this method.
void Updater::writeMemory(DataSource::Segment *segment)
{
    // Inject the write-memory(segment) command.
    WriteMemory cmd(segment, m_memoryId);
    cmd.registerProgress(&m_progress);
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if (fw_status != kStatus_Success)
    {
        throw std::runtime_error(fw_msg);
    }
}

// See Updater.h for documentation of this method.
void Updater::writeMemory(uint32_t address, const uchar_vector_t &data)
{
    // Inject the write-memory(segment) command.
    WriteMemory cmd(address, data, m_memoryId);
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if (fw_status != kStatus_Success)
    {
        throw std::runtime_error(fw_msg);
    }
}

// See Updater.h for documentation of this method.
status_t Updater::flashFromSourceFile()
{
    if (!isMemorySupported(m_memoryId))
    {
        throw std::runtime_error("Selected memory isn't supported!");
    }

    m_sourceFile->open();

    DataSource *dataSource = m_sourceFile->createDataSource();

    m_progress.m_segmentCount = dataSource->getSegmentCount();

    for (uint32_t index = 0; index < dataSource->getSegmentCount(); ++index)
    {
        DataSource::Segment *segment = dataSource->getSegmentAt(index);

        if (segment->hasNaturalLocation())
        {
            dataSource->setTarget(new NaturalDataTarget());
        }
        else
        {
            dataSource->setTarget(new ConstantDataTarget(m_base_address));
        }
        try
        {
            m_operation.current_task = 0;
            m_operation.tasks[m_operation.current_task].current += segment->getLength();
            if (m_progressCallback)
            {
                m_progressCallback(&m_operation);
            }
            eraseFlashRegion(segment->getBaseAddress(), segment->getLength());
        }
        catch (const std::runtime_error &e)
        {
            // If the target region is not flash, bootloader will return kStatus_FlashAddressError.
            // Ignore this error, and skip to next segment
            if (strcmp(e.what(), Command::getStatusMessage(102 /*kStatus_FlashAddressError*/).c_str()) != 0)
            {
                throw e;
            }
        }
    }
    for (uint32_t index = 0; index < dataSource->getSegmentCount(); ++index)
    {
        DataSource::Segment *segment = dataSource->getSegmentAt(index);

        // Write the file to the base address.
        m_operation.current_task = 1;
        m_operation.tasks[m_operation.current_task].current += segment->getLength();
        if (m_progressCallback)
        {
            m_progressCallback(&m_operation);
        }
        m_progress.m_segmentIndex = index + 1;
        writeMemory(segment);
    }

    delete dataSource;
    m_sourceFile->close();

    return kStatus_Success;
}

// See Updater.h for documentation of this method.
status_t Updater::flashFromSBFile(const char *filename)
{
    if (!isCommandSupported(kCommand_ReceiveSbFile))
    {
        throw std::runtime_error("ReceiveSbFile isn't supported!");
    }
    // Inject the receive-sb-file command.
    m_operation.current_task = 1;
    if (m_progressCallback)
    {
        m_progressCallback(&m_operation);
    }
    ReceiveSbFile cmd(filename);
    cmd.registerProgress(&m_progress);
    Log::info("inject command '%s'\n", cmd.getName().c_str());
    inject(cmd);

    // print command response values using the Logger.
    cmd.logResponses();

    uint32_t fw_status = cmd.getResponseValues()->at(0);
    std::string fw_msg = cmd.getStatusMessage(fw_status);

    // Check the command status
    if ((fw_status != kStatus_Success) && (fw_status != kStatus_AbortDataPhase))
    {
        throw std::runtime_error(fw_msg);
    }
    return fw_status;
}

bool Updater::isMemorySupported(uint32_t memoryId)
{
    uint32_t versionNumber = (m_version.major << 16) | (m_version.minor << 8) | (m_version.bugfix << 0);

    switch (memoryId)
    {
        case kMemoryInternal:
        {
            // Internal memory is supported by all devices running Kinetis bootloader.
            return true;
        }
        break;
        case kMemoryQuadSpi0:
        {
            // Bootloader 2.0.0 and previous versions only support internal and QSPI0.
            if (versionNumber <= 0x20000 /*KBL2.0.0*/)
            {
                return isCommandSupported(kCommand_ConfigureMemory);
            }
            else
            {
                GetProperty getExernalAttri(kProperty_ExernalMemoryAttributes, kMemoryQuadSpi0);
                inject(getExernalAttri);
                status_t fw_status = getExernalAttri.getResponseValues()->at(0);
                // UnknownProperty means that no external memories are supported by current device.
                // InvalidArgument means that current memory type is not supported by the device.
                if ((fw_status == kStatus_UnknownProperty) || (fw_status == kStatus_InvalidArgument))
                {
                    return false;
                }
                else if ((fw_status == 405 /*kStatus_QspiNotConfigured*/) || (fw_status == kStatus_Success))
                {
                    return true;
                }
                else
                {
                    // Failed to get property of the device memory.
                    throw std::runtime_error("Failed to check the memory ID!");
                }
            }
        }
        break;
        case kMemoryIFR0:
        {
            return isCommandSupported(kCommand_FlashProgramOnce);
        }
        break;
        case kMemoryFlashExecuteOnly:
        {
            // For the devices that doesn't support erasing all execute-only regions,
            // flash-erase-all command will be executed instead.
            return true;
        }
        break;
        default:
        {
            // Bootloader 2.0.0 and previous versions only support internal and QSPI0.
            if (versionNumber <= 0x20000 /*KBL2.0.0*/)
            {
                return false;
            }
            else
            {
                // If ConfigureMemory is not supported,
                // then no external memory devices are supported by current target.
                if (!isCommandSupported(kCommand_ConfigureMemory))
                {
                    return false;
                }
                GetProperty getExernalAttri(kProperty_ExernalMemoryAttributes, memoryId);
                inject(getExernalAttri);
                status_t fw_status = getExernalAttri.getResponseValues()->at(0);
                // UnknownProperty means that no external memories are supported by current device.
                // InvalidArgument means that current memory type is not supported by the device.
                if ((fw_status == kStatus_UnknownProperty) || (fw_status == kStatus_InvalidArgument))
                {
                    return false;
                }
                // Memories, except QSPI0, have a common un-configured status.
                else if ((fw_status == kStatusMemoryNotConfigured) || (fw_status == kStatus_Success))
                {
                    // Supported.
                    return true;
                }
                else
                {
                    // Failed to get property of the device memory.
                    throw std::runtime_error("Failed to check the memory ID!");
                }
            }
        }
        break;
    }
}
