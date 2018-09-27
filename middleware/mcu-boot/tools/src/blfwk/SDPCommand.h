/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SDPCommand_h_
#define _SDPCommand_h_

#pragma once

#include "host_types.h"
#include "Progress.h"
#include "Packetizer.h"

//! @addtogroup sdp_commands
//! @{

using namespace std;

namespace blfwk
{
//! @brief SDP Command structure.
struct sdp_cmd_t
{
    uint16_t cmdType;
    const char *const name;

    sdp_cmd_t(uint16_t cmdType, const char *name)
        : cmdType(cmdType)
        , name(name)
    {
    }
};

//! @name SDP command types and names.
//@{
const sdp_cmd_t kSDPCommand_ReadRegister(0x0101, "read-register");
const sdp_cmd_t kSDPCommand_WriteRegister(0x0202, "write-register");
const sdp_cmd_t kSDPCommand_WriteFile(0x0404, "write-file");
const sdp_cmd_t kSDPCommand_ErrorStatus(0x0505, "error-status");
const sdp_cmd_t kSDPCommand_DcdWrite(0x0a0a, "dcd-write");
const sdp_cmd_t kSDPCommand_SkipDcdHeader(0x0c0c, "skip-dcd-header");
const sdp_cmd_t kSDPCommand_JumpAddress(0x0b0b, "jump-address");
const sdp_cmd_t kSDPCommand_SetBaudrate(0x0d0d, "set-baudrate");
const sdp_cmd_t kSDPCommand_Ping(0x5aa6, "ping");
//@}

/*!
* @brief Represents an SDP command.
*
* Do not instantiate this class. Instead, use the create() method to
* create the appropriate subclass based on command name in argv[0].
*/
class SDPCommand
{
public:
    //! @brief Constants
    enum _constants
    {
        kCmdSizeBytes = 16,              //!< Number of bytes in SDP command.
        kHabModeSizeBytes = 4,           //!< Number of bytes in HAB mode response.
        kResponseSizeBytes = 64,         //!< Number of bytes in status response.
        kSendDataSizeBytes = 1024,       //!< Number of bytes in data packet.
        kMaxDcdSizeBytes = 1024,         //!< Max number bytes in DCD table.
        kHabEnabled = 0x12343412,        //!< HAB mode enabled.
        kHabDisabled = 0x56787856,       //!< HAB mode disabled.
        kWriteComplete = 0x128a8a12,     //!< Write Complete status response.
        kWriteFileComplete = 0x88888888, //!< Write File Complete status response.
        kHabStatusFailure = 0x33333333,  //!< HAB failed response.
        kHabStatusWarning = 0x69696969,  //!< HAB wrarning response.
        kHabStatusSuccess = 0xf0f0f0f0,  //!< HAB success resopnse.
        kReEnum_Ack = 0x89232389,        //!< Return Error Number Ack
        kStatus_NoResponse = 10004,      //!< Matches Bootloader kStatusNoResponse.
        kOk_Ack = 0x900DD009,            //!< Response to skip-dcd-header command.
    };

public:
    //! @brief Create an appropriate command subclass.
    //!
    //! Pass the command name in argv[0] and optional
    //! arguments in the rest of the string vector.
    //! Caller is responsible for deleting the returned object.
    //!
    //! @param argv Argument vector
    //! @retval Command object
    static SDPCommand *create(const string_vector_t *argv);

protected:
    //! @brief Constructor that takes a command name and list of arguments.
    //!
    //! @param argv Argument vector
    SDPCommand(const string_vector_t *argv)
        : m_argv(*argv)
        , m_progress()
        , m_responseValues()
        , m_cmdBuf()
    {
    }

public:
    //! @brief Destructor.
    virtual ~SDPCommand() {}
    //! @brief Initialize.
    //!
    //! Subclasses should implement init() to check for valid arguments.
    virtual bool init() { return true; }
    //! @name String arguments accessors.
    //@{

    //! @brief Get the specified argument.
    virtual std::string getArg(int arg) const { return m_argv.at(arg); }
    //! @brief Get the command name (i.e. argv[0]).
    virtual std::string getName() const { return getArg(0); }
    //! @brief Get the number of arguments, including the command name.
    virtual size_t getArgCount() const { return m_argv.size(); }
    //@}

    //! @brief Send command to packetizer and on to peripheral.
    virtual void sendTo(Packetizer &packetizer) {}
    //! @brief Get response values vector.
    virtual const uint32_vector_t *getResponseValues() const
    {
        return const_cast<uint32_vector_t *>(&m_responseValues);
    }

    //! @brief Get response as JSON string.
    virtual std::string getResponse() const;

    //! @brief Log the response description.
    virtual void logResponses() const;

    //! @brief Register an object for displaying transfer progress.
    virtual void registerProgress(Progress *progress) { m_progress = progress; }
protected:
    //! @brief Create packed buffer with SDP command.
    void packCommand(uint16_t cmdType, uint32_t address, uint32_t format, uint32_t dataCount, uint32_t data);

    //! @brief Send command to packetizer.
    status_t sendCommand(Packetizer &packetizer);

    //! @brief Send command and read HAB mode response.
    status_t sendCommandGetHabMode(Packetizer &packetizer);

    //! @brief Read response value from device.
    //!
    //! Use kPacketTypeData for 64-byte read-register response, kPacketTypeCommand for 4-byte status response and HAB
    //! mode response.
    uint8_t *getStatusResponse(Packetizer &packetizer, packet_type_t type = kPacketType_Command);

    //! @brief Read HAB mode response from device.
    bool isHabModeValid(Packetizer &packetizer);

protected:
    string_vector_t m_argv;           //!< Vector of argument strings.
    Progress *m_progress;             //!< Variable for progress control.
    uint32_vector_t m_responseValues; //!< Vector of response values.
    uint8_t m_cmdBuf[kCmdSizeBytes];  //!< Buffer for command bytes.
};

/*!
* @brief Represents the SDP ReadRegister command.
*
*/
class SDPReadRegister : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPReadRegister(const string_vector_t *argv)
        : SDPCommand(argv)
        , m_address(0)
        , m_format(32)
        , m_dataCount(4)
        , m_file()
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    uint32_t m_address;   //!< Target memory address.
    uint32_t m_format;    //!< Register format.
    uint32_t m_dataCount; //!< Number of bytes to read.
    std::string m_file;   //!< Data file path.
};

/*!
* @brief Represents the SDP WriteRegister command.
*
*/
class SDPWriteRegister : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPWriteRegister(const string_vector_t *argv)
        : SDPCommand(argv)
        , m_address(0)
        , m_format(32)
        , m_data(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    uint32_t m_address; //!< Target memory address.
    uint32_t m_format;  //!< Register format.
    uint32_t m_data;    //!< Data value to write.
};

/*!
* @brief Represents the SDP WriteFile command.
*
*/
class SDPWriteFile : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPWriteFile(const string_vector_t *argv)
        : SDPCommand(argv)
        , m_address(0)
        , m_file()
        , m_dataCount(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    uint32_t m_address;   //!< Target memory address.
    std::string m_file;   //!< Data file path.
    uint32_t m_dataCount; //!< Number of bytes to write.
};

/*!
* @brief Represents the SDP ErrorStatus command.
*
*/
class SDPErrorStatus : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPErrorStatus(const string_vector_t *argv)
        : SDPCommand(argv)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);
};

/*!
* @brief Represents the SDP DcdWrite command.
*
*/
class SDPDcdWrite : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPDcdWrite(const string_vector_t *argv)
        : SDPCommand(argv)
        , m_address(0)
        , m_file()
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    uint32_t m_address; //!< Temporary storage address.
    std::string m_file; //!< Data file path.
};

/*!
* @brief Represents the SDP SkipDcdHeader command.
*
*/
class SDPSkipDcdHeader : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPSkipDcdHeader(const string_vector_t *argv)
        : SDPCommand(argv)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);
};

/*!
* @brief Represents the SDP JumpAddress command.
*
*/
class SDPJumpAddress : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPJumpAddress(const string_vector_t *argv)
        : SDPCommand(argv)
        , m_address(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    uint32_t m_address; //!< Target memory address.
};

/*!
* @brief REpresents the SDP SetBaudrate command.
*
*/
class SDPSetBaudrate : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPSetBaudrate(const string_vector_t *argv)
        : SDPCommand(argv)
        , m_baudrate(115200)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    uint32_t m_baudrate; //!< Baudrate
};

/*!
* @brief Represents the SDP SkipDcdHeader command.
*
*/
class SDPPing : public SDPCommand
{
public:
    //! @brief Constructor that takes argument vector.
    SDPPing(const string_vector_t *argv)
        : SDPCommand(argv)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);
};

} // namespace blfwk

//! @}

#endif // _SDPCommand_h_
