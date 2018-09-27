/*
* Copyright (c) 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "blfwk/SDPCommand.h"
#include "blfwk/utils.h"
#include "blfwk/Logging.h"
#include "blfwk/json.h"
#include "blfwk/format_string.h"

#include <string.h>

using namespace blfwk;
using namespace utils;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

/*!
* @brief Abstract class to consume data from data phase.
*/
class SDPDataConsumer
{
public:
    //! @brief Process the next data chunk.
    virtual void processData(const uint8_t *data, uint32_t size) = 0;

    //! @brief Finalize processing.
    virtual void finalize() = 0;
};

/*!
* @brief Write file data for data phase receive.
*/
class SDPFileDataConsumer : public SDPDataConsumer
{
public:
    //! @brief Default constructor.
    SDPFileDataConsumer()
        : m_filePath()
        , m_filePointer(NULL)
    {
    }

    //! @brief Destructor.
    virtual ~SDPFileDataConsumer()
    {
        if (m_filePointer)
            fclose(m_filePointer);
    }

    //! @brief Initialize with a file path.
    bool init(std::string filePath);

    //! @brief Process the next data chunk.
    virtual void processData(const uint8_t *data, uint32_t size);

    //! @brief Finalize processing.
    virtual void finalize() {}
protected:
    std::string m_filePath; //!< Data file path.
    FILE *m_filePointer;    //!< Data file pointer.
};

class SDPStdOutDataConsumer : public SDPDataConsumer
{
public:
    //! @brief Constants.
    enum _constants
    {
        kBytesPerLine = 16 //!< Number of hex bytes to display per line
    };

public:
    //! @brief Constructor.
    SDPStdOutDataConsumer()
        : m_currentCount(1)
    {
    }

    //! @brief Finalize processing.
    virtual void finalize()
    {
        if (((m_currentCount - 1) % kBytesPerLine) != 0)
        {
            // Fill space to clean the progress text.
            for (int i = ((m_currentCount - 1) % kBytesPerLine) * 3; i < 9; i++)
            {
                Log::info(" ");
            }
            Log::info("\n");
        }
    }

    //! @brief Process the next data chunk.
    virtual void processData(const uint8_t *data, uint32_t size);

protected:
    uint32_t m_currentCount; //!< Current byte being processed, starts at 1
};

class SDPFileDataProducer
{
public:
    //! @brief Default constructor.
    SDPFileDataProducer()
        : m_filePath()
        , m_filePointer(NULL)
        , m_fileSize(0)
    {
    }

    //! @brief Destructor.
    virtual ~SDPFileDataProducer()
    {
        if (m_filePointer)
            fclose(m_filePointer);
    }

    //! @brief Initialize with a file path.
    bool init(std::string filePath, uint32_t count);

    //! \name DataProducer
    //@{
    //! @brief Query if more data is available.
    virtual bool hasMoreData() const
    {
        assert(m_filePointer);
        return (m_fileSize && !feof(m_filePointer));
    }

    //! @brief Query the total size of the data.
    virtual uint32_t getDataSize() const { return (uint32_t)m_fileSize; }
    //! @brief Get the next data chunk.
    //!
    //! Before calling getData(), call moreData() to determine if
    //! data is available.
    virtual uint32_t getData(uint8_t *data, uint32_t size);
    //@}

    //! @brief Send next packet to packetizer.
    status_t sendPacketTo(Packetizer &packetizer, uint32_t *bytesWritten, Progress *progress);

    //! @brief Send all packets to packetizer.
    status_t sendTo(Packetizer &packetizer, uint32_t *bytesWritten, Progress *progress);

protected:
    std::string m_filePath;                            //!< Data file path.
    FILE *m_filePointer;                               //!< Data file pointer.
    long m_fileSize;                                   //!< Size in bytes of data file.
    uint8_t m_dataBuf[SDPCommand::kSendDataSizeBytes]; //!< Buffer for data bytes.
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Entry in a lookup table of status messages.
//!
//! This struct maps a status value to a description of that status.
struct StatusMessageTableEntry
{
    int32_t status;            //!< Status code value.
    const std::string message; //!< Description of the status.
};

//! @brief Value of the terminator code in the g_statusCodes table.
const int32_t kStatusMessageTableTerminatorValue = 0x7fffffff;

//! @brief Status return code descriptions.
StatusMessageTableEntry g_statusCodes[] = {
    // Generic statuses.
    { kStatus_Success, "Success." },
    { kStatus_Fail, "Failure." },
    { SDPCommand::kHabEnabled, "HAB enabled." },
    { SDPCommand::kHabDisabled, "HAB disabled." },
    { SDPCommand::kWriteComplete, "Write complete." },
    { SDPCommand::kWriteFileComplete, "Write File complete." },
    { SDPCommand::kHabStatusFailure, "HAB failure." },
    { SDPCommand::kHabStatusWarning, "HAB warning." },
    { SDPCommand::kHabStatusSuccess, "HAB Success." },
    { SDPCommand::kStatus_NoResponse, "No response from device." },
    { SDPCommand::kOk_Ack, "OK ACK." },
    { SDPCommand::kReEnum_Ack, "REENUM Ack" },

    // Terminator
    { kStatusMessageTableTerminatorValue, "" }
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

std::string getStatusMessage(status_t code)
{
    int i;
    for (i = 0; g_statusCodes[i].status != kStatusMessageTableTerminatorValue; ++i)
    {
        if (code == g_statusCodes[i].status)
        {
            return g_statusCodes[i].message;
        }
    }

    return format_string("Unknown error code (%u)", code);
}

void SDPStdOutDataConsumer::processData(const uint8_t *data, uint32_t size)
{
    Log::info("\r");

    for (int i = 0; i < (int)size; ++i)
    {
        Log::info("%02x", data[i]);
        if ((m_currentCount++ % kBytesPerLine) == 0)
        {
            Log::info("\n");
        }
        else
        {
            Log::info(" ");
        }
    }
}

bool SDPFileDataProducer::init(string filePath, uint32_t count)
{
    m_filePointer = fopen(filePath.c_str(), "rb");
    if (!m_filePointer)
    {
        Log::error("Error: cannot open input data file '%s'.\n", filePath.c_str());
        return false;
    }

    // If the fileSize wasn't specified, get the file size.
    if (count)
    {
        m_fileSize = count;
    }
    else
    {
        ::fseek(m_filePointer, 0, SEEK_END);
        m_fileSize = ftell(m_filePointer);
        ::fseek(m_filePointer, 0, SEEK_SET);
    }

    Log::info("Preparing to send %u (0x%x) bytes to the target.\n", m_fileSize, m_fileSize);
    return true;
}

//! See host_command.h for documentation on this function.
uint32_t SDPFileDataProducer::getData(uint8_t *data, uint32_t size)
{
    assert(m_filePointer);
    assert(data);
    if ((size == 0) || !hasMoreData())
    {
        return 0;
    }

    return (uint32_t)fread(data, 1, size, m_filePointer);
}

status_t SDPFileDataProducer::sendPacketTo(Packetizer &packetizer, uint32_t *bytesWritten, Progress *progress)
{
    assert(bytesWritten);
    *bytesWritten = 0;

    uint32_t count = getData(m_dataBuf, sizeof(m_dataBuf));
    if (count)
    {
        status_t status = packetizer.writePacket(m_dataBuf, count, kPacketType_Data);
        if (status != kStatus_Success)
        {
            Log::error("SDPFileDataProducer.writePacket error %u.\n", status);
            return status;
        }

        *bytesWritten += count;

        if (progress != NULL)
        {
            // Execute process callback function.
            progress->progressCallback(*bytesWritten * 100 / getDataSize());
        }
    }

    return kStatus_Success;
}

status_t SDPFileDataProducer::sendTo(Packetizer &packetizer, uint32_t *bytesWritten, Progress *progress)
{
    assert(bytesWritten);
    *bytesWritten = 0;

    while (hasMoreData() && *bytesWritten < getDataSize())
    {
        uint32_t packetBytesWritten;
        status_t status = sendPacketTo(packetizer, &packetBytesWritten, progress);
        if (status != kStatus_Success)
        {
            return status;
        }

        *bytesWritten += packetBytesWritten;
    }

    return kStatus_Success;
}

bool SDPFileDataConsumer::init(string filePath)
{
    m_filePointer = fopen(filePath.c_str(), "wb");
    if (!m_filePointer)
    {
        Log::error("Error: cannot open output data file '%s'.\n", filePath.c_str());
        return false;
    }
    return true;
}

void SDPFileDataConsumer::processData(const uint8_t *data, uint32_t size)
{
    fwrite(data, 1, size, m_filePointer);
}

SDPCommand *SDPCommand::create(const string_vector_t *argv)
{
    SDPCommand *cmd;

    if (argv->at(0) == kSDPCommand_ReadRegister.name)
    {
        cmd = new SDPReadRegister(argv);
    }
    else if (argv->at(0) == kSDPCommand_WriteRegister.name)
    {
        cmd = new SDPWriteRegister(argv);
    }
    else if (argv->at(0) == kSDPCommand_WriteFile.name)
    {
        cmd = new SDPWriteFile(argv);
    }
    else if (argv->at(0) == kSDPCommand_ErrorStatus.name)
    {
        cmd = new SDPErrorStatus(argv);
    }
    else if (argv->at(0) == kSDPCommand_DcdWrite.name)
    {
        cmd = new SDPDcdWrite(argv);
    }
    else if (argv->at(0) == kSDPCommand_SkipDcdHeader.name)
    {
        cmd = new SDPSkipDcdHeader(argv);
    }
    else if (argv->at(0) == kSDPCommand_JumpAddress.name)
    {
        cmd = new SDPJumpAddress(argv);
    }
    else if (argv->at(0) == kSDPCommand_SetBaudrate.name)
    {
        cmd = new SDPSetBaudrate(argv);
    }
    else if (argv->at(0) == kSDPCommand_Ping.name)
    {
        cmd = new SDPPing(argv);
    }
    else
    {
        return NULL;
    }

    // Validate arguments.
    if (!cmd->init())
    {
        delete cmd;
        return NULL;
    }

    return cmd;
}

void SDPCommand::packCommand(uint16_t cmdType, uint32_t address, uint32_t format, uint32_t dataCount, uint32_t data)
{
    memset(m_cmdBuf, 0, kCmdSizeBytes);
    uint32_t *cmd = (uint32_t *)m_cmdBuf;

    cmd[0] = (((address & 0x00FF0000) << 8) | ((address & 0xFF000000) >> 8) | (cmdType & 0x0000FFFF));

    cmd[1] = ((dataCount & 0xFF000000) | ((format & 0x000000FF) << 16) | ((address & 0x000000FF) << 8) |
              ((address & 0x0000FF00) >> 8));

    cmd[2] = ((data & 0xFF000000) | ((dataCount & 0x000000FF) << 16) | (dataCount & 0x0000FF00) |
              ((dataCount & 0x00FF0000) >> 16));

    cmd[3] =
        (((0x00 & 0x000000FF) << 24) | ((data & 0x00FF0000) >> 16) | (data & 0x0000FF00) | ((data & 0x000000FF) << 16));
}

status_t SDPCommand::sendCommand(Packetizer &packetizer)
{
    status_t status = kStatus_Success;

    status = packetizer.writePacket(m_cmdBuf, kCmdSizeBytes, kPacketType_Command);
    if (status != kStatus_Success)
    {
        Log::error("sendCommand.writePacket error %u.\n", status);
        return status;
    }

    return kStatus_Success;
}

status_t SDPCommand::sendCommandGetHabMode(Packetizer &packetizer)
{
    status_t status = kStatus_Success;

    // Send the command.
    status = sendCommand(packetizer);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Read and validate HAB mode response.
    return isHabModeValid(packetizer) ? kStatus_Success : kStatus_Fail;
}

uint8_t *SDPCommand::getStatusResponse(Packetizer &packetizer, packet_type_t type)
{
    status_t status;

    // Read status/response.
    uint8_t *response = NULL;
    uint32_t responseSize = 0;
    status = packetizer.readPacket(&response, &responseSize, type);
    if (status != kStatus_Success)
    {
        Log::error("getStatusResponse.readPacket error %u.\n", status);
        return NULL;
    }

    return response;
}

bool SDPCommand::isHabModeValid(Packetizer &packetizer)
{
    uint8_t *habMode = getStatusResponse(packetizer);
    if (!habMode)
    {
        m_responseValues.push_back(kStatus_NoResponse);
        return false;
    }

    uint32_t intMode = *((uint32_t *)habMode);
    m_responseValues.push_back(intMode);

    if ((intMode == kHabDisabled) || (intMode == kHabEnabled))
    {
        return true;
    }
    else
    {
        Log::error("unknown HAB mode %u.\n", intMode);
        return false;
    }

    return true;
}

std::string SDPCommand::getResponse() const
{
    size_t count = m_responseValues.size();

    Json::Value root;
    root["command"] = getName();
    root["status"] = Json::Value(Json::objectValue);
    root["status"]["value"] = static_cast<int32_t>(m_responseValues.at(0));
    root["status"]["description"] = format_string("%u (0x%X) %s", m_responseValues.at(0), m_responseValues.at(0),
                                                  getStatusMessage(m_responseValues.at(0)).c_str());
    root["response"] = Json::Value(Json::arrayValue);
    for (int i = 1; i < (int)m_responseValues.size(); ++i)
    {
        root["response"].append(Json::Value(m_responseValues.at(i)));
    }

    Json::StyledWriter writer;
    return writer.write(root);
}

void SDPCommand::logResponses() const
{
    const uint32_vector_t *respv = getResponseValues();
    size_t count = respv->size();

    if (getName() == kSDPCommand_SetBaudrate.name)
    {
        Log::info("Status = %u (0x%x) %s\n", respv->at(0), respv->at(0), getStatusMessage(respv->at(0)).c_str());
    }
    else if (getName() == kSDPCommand_Ping.name)
    {
        Log::info("Status (HAB mode) = %u (0x%x) %s\n", respv->at(0), respv->at(0),
                  getStatusMessage(respv->at(0)).c_str());
        Log::info("Ping response, Device ID = 0x%08x 0x%08x\n", respv->at(1), respv->at(2));
    }
    else
    {
        Log::info("Status (HAB mode) = %u (0x%x) %s\n", respv->at(0), respv->at(0),
                  getStatusMessage(respv->at(0)).c_str());
        if (count > 1)
        {
            Log::info("Reponse Status = %u (0x%x) %s\n", respv->at(1), respv->at(1),
                      getStatusMessage(respv->at(1)).c_str());
        }
    }

    Log::json(getResponse().c_str());
}

////////////////////////////////////////////////////////////////////////////////
// SDP ReadRegister command
////////////////////////////////////////////////////////////////////////////////

bool SDPReadRegister::init()
{
    if ((getArgCount() < 2) || (getArgCount() > 5))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_address))
    {
        return false; // invalid 'addr' parameter
    }

    // Default format and count.
    m_format = 32;
    m_dataCount = m_format / 8;

    if (getArgCount() > 2)
    {
        if (!utils::stringtoui(getArg(2), m_format))
        {
            return false; // invalid 'format' parameter
        }
        if (!((m_format == 8) || (m_format == 16) || (m_format == 32)))
        {
            return false; // invaid 'format' parameter
        }

        // Default count.
        m_dataCount = m_format / 8;

        if (getArgCount() > 3)
        {
            if (!utils::stringtoui(getArg(3), m_dataCount))
            {
                return false; // invalid 'count' parameter
            }

            if (m_dataCount == 0)
            {
                return false; // invalid 'count' parameter
            }

            if (getArgCount() == 5)
            {
                m_file = getArg(4);
            }
        }
    }

    return true;
}

void SDPReadRegister::sendTo(Packetizer &packetizer)
{
    packCommand(kSDPCommand_ReadRegister.cmdType, m_address, m_format, m_dataCount, 0);
    if (sendCommandGetHabMode(packetizer) != kStatus_Success)
    {
        return;
    }

    SDPDataConsumer *printer;
    if (m_file.size() > 0)
    {
        SDPFileDataConsumer *filePrinter = new SDPFileDataConsumer;
        if (!filePrinter->init(m_file))
        {
            delete filePrinter;
            return;
        }
        printer = filePrinter;
    }
    else
    {
        printer = new SDPStdOutDataConsumer;
    }

    // Display response values.
    int32_t remaining = m_dataCount;
    do
    {
        int32_t requested = MIN(remaining, kResponseSizeBytes);

        // Tell the packetizer how many bytes to read from the peripheral.
        packetizer.setReadCount(requested);

        uint8_t *data = getStatusResponse(packetizer, kPacketType_Data);
        if (!data)
        {
            break;
        }

        printer->processData(data, requested);

        remaining -= kResponseSizeBytes;
    } while (remaining > 0);

    printer->finalize();
    delete printer;
}

////////////////////////////////////////////////////////////////////////////////
// SDP WriteRegister command
////////////////////////////////////////////////////////////////////////////////

bool SDPWriteRegister::init()
{
    if (getArgCount() != 4)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_address))
    {
        return false; // invalid 'addr' parameter
    }
    if (!utils::stringtoui(getArg(2), m_format))
    {
        return false; // invalid 'format' parameter
    }
    if (!utils::stringtoui(getArg(3), m_data))
    {
        return false; // invalid 'data' parameter
    }

    if (!((m_format == 8) || (m_format == 16) || (m_format == 32)))
    {
        return false; // invaid 'format' parameter
    }

    return true;
}

void SDPWriteRegister::sendTo(Packetizer &packetizer)
{
    packCommand(kSDPCommand_WriteRegister.cmdType, m_address, m_format, 0, m_data);
    if (sendCommandGetHabMode(packetizer) != kStatus_Success)
    {
        return;
    }

    // Check for WRITE_COMPLETE status.
    uint8_t *data = getStatusResponse(packetizer);
    if (!data)
    {
        return;
    }
    uint32_t intData = *((uint32_t *)data);
    m_responseValues.push_back(intData);
    if (intData != kWriteComplete)
    {
        Log::error("unexpected write response 0x%x.\n", intData);
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
// SDP WriteFile command
////////////////////////////////////////////////////////////////////////////////

bool SDPWriteFile::init()
{
    if ((getArgCount() < 3) || (getArgCount() > 4))
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_address))
    {
        return false; // invalid 'addr' parameter
    }

    m_file = getArg(2);

    if (getArgCount() == 4)
    {
        if (!utils::stringtoui(getArg(3), m_dataCount))
        {
            return false; // invalid 'count' parameter
        }

        if (m_dataCount == 0)
        {
            return false; // invalid 'count' parameter
        }
    }

    return true;
}

void SDPWriteFile::sendTo(Packetizer &packetizer)
{
    SDPFileDataProducer fileProducer;

    if (!fileProducer.init(m_file, m_dataCount))
    {
        return;
    }

    if (fileProducer.getDataSize() == 0)
    {
        Log::error("non-zero file size required.\n");
        return;
    }

    // Send the write file command.
    packCommand(kSDPCommand_WriteFile.cmdType, m_address, 0, fileProducer.getDataSize(), 0);
    if (sendCommand(packetizer) != kStatus_Success)
    {
        return;
    }

    // Write the file data.
    uint32_t bytesWritten;
    if (fileProducer.sendTo(packetizer, &bytesWritten, m_progress) != kStatus_Success)
    {
        return;
    }

    // Read and verify hab mode.
    if (!isHabModeValid(packetizer))
    {
        return;
    }

    // Check for COMPLETE status.
    uint8_t *data = getStatusResponse(packetizer);
    if (!data)
    {
        return;
    }
    uint32_t intData = *((uint32_t *)data);
    m_responseValues.push_back(intData);
    if (intData != kWriteFileComplete)
    {
        Log::error("unexpected write file response 0x%x.\n", intData);
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
// SDP ErrorStatus command
////////////////////////////////////////////////////////////////////////////////

bool SDPErrorStatus::init()
{
    if (getArgCount() != 1)
    {
        return false;
    }

    return true;
}

void SDPErrorStatus::sendTo(Packetizer &packetizer)
{
    packCommand(kSDPCommand_ErrorStatus.cmdType, 0, 0, 0, 0);
    if (sendCommandGetHabMode(packetizer) != kStatus_Success)
    {
        return;
    }

    // Read status response.
    uint8_t *data = getStatusResponse(packetizer);
    if (!data)
    {
        return;
    }
    uint32_t intData = *((uint32_t *)data);
    m_responseValues.push_back(intData);
}

////////////////////////////////////////////////////////////////////////////////
// SDP DcdWrite command
////////////////////////////////////////////////////////////////////////////////

bool SDPDcdWrite::init()
{
    if (getArgCount() != 3)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_address))
    {
        return false; // invalid 'addr' parameter
    }

    m_file = getArg(2);

    return true;
}

void SDPDcdWrite::sendTo(Packetizer &packetizer)
{
    uint32_t dataCount = 0;
    SDPFileDataProducer fileProducer;

    if (!fileProducer.init(m_file, dataCount))
    {
        return;
    }

    uint32_t dataSize = fileProducer.getDataSize();
    if ((dataSize == 0) || (dataSize > kMaxDcdSizeBytes))
    {
        Log::error("DCD file size must be non-zero and <= %d bytes.\n", kMaxDcdSizeBytes);
        return;
    }

    // Send the DCD Write command.
    packCommand(kSDPCommand_DcdWrite.cmdType, m_address, 0, dataSize, 0);
    if (sendCommand(packetizer) != kStatus_Success)
    {
        return;
    }

    // Write the file data.
    uint32_t bytesWritten;
    if (fileProducer.sendTo(packetizer, &bytesWritten, m_progress) != kStatus_Success)
    {
        return;
    }

    // Read and verify hab mode.
    if (!isHabModeValid(packetizer))
    {
        return;
    }

    // Check for COMPLETE status.
    uint8_t *data = getStatusResponse(packetizer);
    if (!data)
    {
        return;
    }
    uint32_t intData = *((uint32_t *)data);
    m_responseValues.push_back(intData);
    if (intData != kWriteComplete)
    {
        Log::error("unexpected write file response 0x%x.\n", intData);
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
// SDP SkipDcdHeader command
////////////////////////////////////////////////////////////////////////////////

bool SDPSkipDcdHeader::init()
{
    if (getArgCount() != 1)
    {
        return false;
    }

    return true;
}

void SDPSkipDcdHeader::sendTo(Packetizer &packetizer)
{
    packCommand(kSDPCommand_SkipDcdHeader.cmdType, 0, 0, 0, 0);
    if (sendCommandGetHabMode(packetizer) != kStatus_Success)
    {
        return;
    }

    // Read status response.
    uint8_t *data = getStatusResponse(packetizer);
    if (!data)
    {
        return;
    }
    uint32_t intData = *((uint32_t *)data);
    m_responseValues.push_back(intData);
}

////////////////////////////////////////////////////////////////////////////////
// SDP JumpAddress command
////////////////////////////////////////////////////////////////////////////////

bool SDPJumpAddress::init()
{
    if (getArgCount() != 2)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_address))
    {
        return false; // invalid 'addr' parameter
    }

    return true;
}

void SDPJumpAddress::sendTo(Packetizer &packetizer)
{
    packCommand(kSDPCommand_JumpAddress.cmdType, m_address, 0, 0, 0);
    sendCommandGetHabMode(packetizer);
}

////////////////////////////////////////////////////////////////////////////////
// SDP SetBaudrate command
////////////////////////////////////////////////////////////////////////////////

bool SDPSetBaudrate::init()
{
    if (getArgCount() != 2)
    {
        return false;
    }

    if (!utils::stringtoui(getArg(1), m_baudrate))
    {
        Log::error("unrecognized baudrate string: %s\n", getArg(1).c_str());
        return false; // invalid 'baudrate' parameter
    }

    return true;
}

void SDPSetBaudrate::sendTo(Packetizer &packetizer)
{
    packCommand(kSDPCommand_SetBaudrate.cmdType, m_baudrate, 0, 0, 0);
    sendCommandGetHabMode(packetizer);
}

////////////////////////////////////////////////////////////////////////////////
// SDP Ping command
////////////////////////////////////////////////////////////////////////////////
bool SDPPing::init()
{
    if (getArgCount() != 1)
    {
        return false;
    }

    return true;
}

void SDPPing::sendTo(Packetizer &packetizer)
{
    uint8_t ping[2] = { 0x5a, 0xa6 };
    uint8_t *response = NULL;
    uint32_t responseSize = 0;
    status_t status = packetizer.writePacket(ping, sizeof(ping), kPacketType_Command);
    if (status != kStatus_Success)
    {
        Log::error("Failed to send ping packet.\n");
        return;
    }

    packetizer.setReadCount(12);
    status = packetizer.readPacket(&response, &responseSize, kPacketType_Data);
    if (status != kStatus_Success)
    {
        Log::error("getPingResponse.readPacket error %u.\n", status);
        return;
    }

    if (response[0] == 0x5a && response[1] == 0xa8)
    {
        uint32_t uuid[2];
        memcpy(&uuid[0], &response[4], sizeof(uuid));
        m_responseValues.push_back(kOk_Ack);
        m_responseValues.push_back(uuid[0]);
        m_responseValues.push_back(uuid[1]);
    }
    else
    {
        Log::error("Invalid ping response.\n");
    }
}
