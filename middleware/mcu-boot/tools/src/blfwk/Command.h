/*
 * Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _Command_h_
#define _Command_h_

#include <array>
#include <assert.h>
#include "DataSource.h"
#include "Packetizer.h"
#include "host_types.h"
#include "format_string.h"
#include "property/property.h"
#include "memory/memory.h"
#include "SourceFile.h"
#include "BusPal.h"
#include "Progress.h"

//! @addtogroup host_commands
//! @{

using namespace std;

namespace blfwk
{
//! @name Command tags, masks, names.
//@{
struct cmd_t
{
    uint8_t tag;
    uint32_t mask;
    const char *const name;

    cmd_t(uint8_t tag, uint32_t mask, const char *name)
        : tag(tag)
        , mask(mask)
        , name(name)
    {
    }
};

const cmd_t kCommand_FlashEraseAll(kCommandTag_FlashEraseAll, 0x00000001, "flash-erase-all");
const cmd_t kCommand_FlashEraseRegion(kCommandTag_FlashEraseRegion, 0x00000002, "flash-erase-region");
const cmd_t kCommand_ReadMemory(kCommandTag_ReadMemory, 0x00000004, "read-memory");
const cmd_t kCommand_WriteMemory(kCommandTag_WriteMemory, 0x00000008, "write-memory");
const cmd_t kCommand_FillMemory(kCommandTag_FillMemory, 0x00000010, "fill-memory");
const cmd_t kCommand_FlashSecurityDisable(kCommandTag_FlashSecurityDisable, 0x00000020, "flash-security-disable");
const cmd_t kCommand_GetProperty(kCommandTag_GetProperty, 0x00000040, "get-property");
const cmd_t kCommand_ReceiveSbFile(kCommandTag_ReceiveSbFile, 0x00000080, "receive-sb-file");
const cmd_t kCommand_Execute(kCommandTag_Execute, 0x00000100, "execute");
const cmd_t kCommand_Call(kCommandTag_Call, 0x00000200, "call");
const cmd_t kCommand_Reset(kCommandTag_Reset, 0x00000400, "reset");
const cmd_t kCommand_SetProperty(kCommandTag_SetProperty, 0x00000800, "set-property");
const cmd_t kCommand_FlashEraseAllUnsecure(kCommandTag_FlashEraseAllUnsecure, 0x00001000, "flash-erase-all-unsecure");
const cmd_t kCommand_FlashProgramOnce(kCommandTag_FlashProgramOnce, 0x00002000, "flash-program-once");
const cmd_t kCommand_FlashReadOnce(kCommandTag_FlashReadOnce, 0x00004000, "flash-read-once");
const cmd_t kCommand_FlashReadResource(kCommandTag_FlashReadResource, 0x00008000, "flash-read-resource");
const cmd_t kCommand_ConfigureMemory(kCommandTag_ConfigureMemory, 0x00010000, "configure-memory");
const cmd_t kCommand_ReliableUpdate(kCommandTag_ReliableUpdate, 0x00100000, "reliable-update");
const cmd_t kCommand_GenerateKeyBlob(kCommandTag_GenerateKeyBlob, 0x00200000, "generate-key-blob");
const cmd_t kCommand_KeyProvisoning(kCommandTag_KeyProvisoning, 0x00400000, "key-provisioning");
const cmd_t kCommand_ConfigureI2c(kCommandTag_ConfigureI2c, 0x00020000, "i2c");
const cmd_t kCommand_ConfigureSpi(kCommandTag_ConfigureSpi, 0x00040000, "spi");
const cmd_t kCommand_ConfigureCan(kCommandTag_ConfigureCan, 0x00080000, "can");
// flash-image and list-memory is not a ROM basic command, but a command sequence combined by several basic commands.
const cmd_t kCommand_FlashImage(0x00, 0x00000000, "flash-image");
const cmd_t kCommand_ListMemory(0x00, 0x00000000, "list-memory");
// efuse-program-once and efuse-read-once are alias of flash-program-once and flash-read-once,
// to avoid the confusion that efuse is not a part of internal flash.
const cmd_t kCommand_EfuseProgramOnce(0x00, 0x00000000, "efuse-program-once");
const cmd_t kCommand_EfuseReadOnce(0x00, 0x00000000, "efuse-read-once");
const cmd_t kCommand_ProgramAESKey(0x00, 0x00000000, "program-aeskey");


const array<const cmd_t, 25> kCommands = { kCommand_FlashEraseAll,
                                           kCommand_FlashEraseRegion,
                                           kCommand_ReadMemory,
                                           kCommand_WriteMemory,
                                           kCommand_FillMemory,
                                           kCommand_FlashSecurityDisable,
                                           kCommand_GetProperty,
                                           kCommand_ReceiveSbFile,
                                           kCommand_Execute,
                                           kCommand_Call,
                                           kCommand_Reset,
                                           kCommand_SetProperty,
                                           kCommand_FlashEraseAllUnsecure,
                                           kCommand_FlashProgramOnce,
                                           kCommand_FlashReadOnce,
                                           kCommand_FlashReadResource,
                                           kCommand_ConfigureMemory,
                                           kCommand_ReliableUpdate,
                                           kCommand_GenerateKeyBlob,
                                           kCommand_KeyProvisoning,
                                           kCommand_FlashImage,
                                           kCommand_ConfigureI2c,
                                           kCommand_ConfigureSpi,
                                           kCommand_ConfigureCan,
                                           kCommand_ListMemory };
//@}

//! @name Property tags.
//@{
struct property_t
{
    uint32_t value;
    const char *description;

    property_t(uint8_t value, const char *description)
        : value(value)
        , description(description)
    {
    }

    string str() const { return format_string("0x%02x", value); }
};
const property_t kProperty_ListProperties(kPropertyTag_ListProperties, "list-properties");
const property_t kProperty_CurrentVersion(kPropertyTag_BootloaderVersion, "current-version");
const property_t kProperty_AvailablePeripherals(kPropertyTag_AvailablePeripherals, "available-peripherals");
const property_t kProperty_FlashStartAddress(kPropertyTag_FlashStartAddress, "flash-start-address");
const property_t kProperty_FlashSizeInBytes(kPropertyTag_FlashSizeInBytes, "flash-size-in-bytes");
const property_t kProperty_FlashSectorSize(kPropertyTag_FlashSectorSize, "flash-sector-size");
const property_t kProperty_FlashBlockCount(kPropertyTag_FlashBlockCount, "flash-block-count");
const property_t kProperty_AvailableCommands(kPropertyTag_AvailableCommands, "available-commands");
const property_t kProperty_CrcCheckStatus(kPropertyTag_CrcCheckStatus, "crc-check-status");
const property_t kProperty_Reserved9(kPropertyTag_Reserved9, "reserved");
const property_t kProperty_VerifyWrites(kPropertyTag_VerifyWrites, "verify-writes");
const property_t kProperty_MaxPacketSize(kPropertyTag_MaxPacketSize, "max-packet-size");
const property_t kProperty_ReservedRegions(kPropertyTag_ReservedRegions, "reserved-regions");
const property_t kProperty_Reserved13(kPropertyTag_Reserved13, "reserved");
const property_t kProperty_RAMStartAddress(kPropertyTag_RAMStartAddress, "ram-start-address");
const property_t kProperty_RAMSizeInBytes(kPropertyTag_RAMSizeInBytes, "ram-size-in-bytes");
const property_t kProperty_SystemDeviceId(kPropertyTag_SystemDeviceId, "system-device-id");
const property_t kProperty_FlashSecurityState(kPropertyTag_FlashSecurityState, "flash-security-state");
const property_t kProperty_UniqueDeviceId(kPropertyTag_UniqueDeviceId, "unique-device-id");
const property_t kProperty_FacSupport(kPropertyTag_FacSupport, "flash-fac-support");
const property_t kProperty_FlashAccessSegmentSize(kPropertyTag_FlashAccessSegmentSize, "flash-access-segment-size");
const property_t kProperty_FlashAccessSegmentCount(kPropertyTag_FlashAccessSegmentCount, "flash-access-segment-count");
const property_t kProperty_FlashReadMargin(kPropertyTag_FlashReadMargin, "flash-read-margin");
const property_t kProperty_QspiInitStatus(kPropertyTag_QspiInitStatus, "qspi/otfad-init-status");
const property_t kProperty_TargetVersion(kPropertyTag_TargetVersion, "target-version");
const property_t kProperty_ExernalMemoryAttributes(kPropertyTag_ExternalMemoryAttributes, "external-memory-attributes");
const property_t kProperty_ReliableUpdateStatus(kPropertyTag_ReliableUpdateStatus, "reliable-update-status");
const property_t kProperty_FlashPageSize(kPropertyTag_FlashPageSize, "flash-page-size");
const property_t kProperty_IrqNotifierPin(kPropertyTag_IrqNotifierPin, "irq-notify-pin");
const property_t kProperty_FfrKeystoreUpdateOpt(kPropertyTag_FfrKeystoreUpdateOpt, "ffr-keystore_update-opt");
const property_t kProperty_Invalid(kPropertyTag_InvalidProperty, "invalid-property");

typedef array<const property_t, 30> PropertyArray;

const PropertyArray kProperties = { kProperty_ListProperties,         kProperty_CurrentVersion,
                                    kProperty_AvailablePeripherals,   kProperty_FlashStartAddress,
                                    kProperty_FlashSizeInBytes,       kProperty_FlashSectorSize,
                                    kProperty_FlashBlockCount,        kProperty_AvailableCommands,
                                    kProperty_CrcCheckStatus,         kProperty_Reserved9,
                                    kProperty_VerifyWrites,           kProperty_MaxPacketSize,
                                    kProperty_ReservedRegions,        kProperty_Reserved13,
                                    kProperty_RAMStartAddress,        kProperty_RAMSizeInBytes,
                                    kProperty_SystemDeviceId,         kProperty_FlashSecurityState,
                                    kProperty_UniqueDeviceId,         kProperty_FacSupport,
                                    kProperty_FlashAccessSegmentSize, kProperty_FlashAccessSegmentCount,
                                    kProperty_FlashReadMargin,        kProperty_QspiInitStatus,
                                    kProperty_TargetVersion,          kProperty_ExernalMemoryAttributes,
                                    kProperty_ReliableUpdateStatus,   kProperty_FlashPageSize,
                                    kProperty_IrqNotifierPin,         kProperty_FfrKeystoreUpdateOpt };
//@}

//! @name Memory IDs.
//@{
struct memory_t
{
    uint32_t memoryId;
    const char *description;

    memory_t(uint32_t memoryId, const char *description)
        : memoryId(memoryId)
        , description(description)
    {
    }
};

const memory_t kMemory_Internal(kMemoryInternal, "Internal");
const memory_t kMemory_QuadSpi(kMemoryQuadSpi0, "QuadSPI");
const memory_t kMemory_IFR0(kMemoryIFR0, "IFR0");
const memory_t kMemory_ExecuteOnly(kMemoryFlashExecuteOnly, "ExecuteOnly");
const memory_t kMemory_SemcNor(kMemorySemcNor, "SEMC NOR");
const memory_t kMemory_FlexSpiNor(kMemoryFlexSpiNor, "FlexSPI NOR");
const memory_t kMemory_SemcNand(kMemorySemcNand, "SEMC NAND");
const memory_t kMemory_SpiNand(kMemorySpiNand, "SPI NAND");
const memory_t kMemory_SpiEeprom(kMemorySpiNorEeprom, "SPI NOR/EEPROM");
const memory_t kMemory_I2cEeprom(kMemoryI2cNorEeprom, "I2C NOR/EEPROM");
const memory_t kMemory_SdCard(kMemorySDCard, "SD/SDHC");
const memory_t kMemory_MmcCard(kMemoryMMCCard, "MMC/eMMC");

typedef array<const memory_t, 9> MemoryArray;

const MemoryArray kMemories = { kMemory_QuadSpi,   kMemory_SemcNor, kMemory_FlexSpiNor,
                                kMemory_SemcNand,  kMemory_SpiNand, kMemory_SpiEeprom,
                                kMemory_I2cEeprom, kMemory_SdCard,  kMemory_MmcCard };
//@}

//! @name Peripheral bits.
//@{
struct peripheral_t
{
    uint32_t mask;
    const char *description;

    peripheral_t(uint32_t mask, const char *description)
        : mask(mask)
        , description(description)
    {
    }
};

const peripheral_t kPeripheral_Uart(0x00000001, "UART");
const peripheral_t kPeripheral_I2C(0x00000002, "I2C Slave");
const peripheral_t kPeripheral_Spi(0x00000004, "SPI Slave");
const peripheral_t kPeripheral_Can(0x00000008, "CAN");
const peripheral_t kPeripheral_UsbHid(0x00000010, "USB HID");
const peripheral_t kPeripheral_UsbCdc(0x00000020, "USB CDC");
const peripheral_t kPeripheral_UsbDfu(0x00000040, "USB DFU");

const array<const peripheral_t, 7> kPeripherals = { kPeripheral_Uart,  kPeripheral_I2C,    kPeripheral_Spi,
                                                    kPeripheral_Can,   kPeripheral_UsbHid, kPeripheral_UsbCdc,
                                                    kPeripheral_UsbDfu };
//@}

//! @brief Entry in a lookup table of status messages.
//!
//! This struct maps a status value to a description of that status.
struct StatusMessageTableEntry
{
    int32_t status;            //!< Status code value.
    const std::string message; //!< Description of the status.
};

//! @brief Status return code descriptions.
//!
//! @warning These strings need to be kept in sync with the platform status codes
//! in the src/include/bootloader_common.h file.
extern StatusMessageTableEntry g_statusCodes[];

//! @name Commands
//@{

/*!
 * @brief Represents a bootloader command.
 *
 * Do not instantiate this class. Instead, use the create() method to
 * create the appropriate subclass based on command name in argv[0].
 */
class Command
{
public:
    //! @brief Create an appropriate command subclass.
    //!
    //! Pass the command name in argv[0] and optional
    //! arguments in the rest of the string vector.
    //! Caller is responsible for deleting the returned object.
    //!
    //! This factory method is just a shorthand for creating
    //! a subclass directly, for example:
    //! @code
    //!  string_vector_t cmdv;
    //!  cmdv.push_back("my-reset");
    //!  Reset * reset = new Reset(&cmdv);
    //!  reset->init();
    //!  Command * cmd = reset;
    //! @endcode
    //!
    //! @param argv Argument vector
    //! @retval Command object
    static Command *create(const string_vector_t *argv);

public:
    //! @brief Get a status code description.
    static std::string getStatusMessage(status_t code);

protected:
    //! @brief Constructor that takes a command name and list of arguments.
    //!
    //! @param argv Argument vector
    Command(const string_vector_t *argv)
        : m_argv(*argv)
        , m_responseValues()
        , m_responseDetails()
        , m_progress()
    {
    }

    //! @brief Constructor that takes a command name.
    //!
    //! @param name Name of the command
    Command(const char *const name)
        : m_argv(1, name)
        , m_responseValues()
        , m_responseDetails()
        , m_progress()
    {
    }

public:
    //! @brief Destructor.
    virtual ~Command() {}
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
    void logResponses() const;

    //! @brief initial the process callback.
    void registerProgress(Progress *progress) { m_progress = progress; }
protected:
    //! @brief Check generic response packet.
    //!
    //! @param packet Packet received from device
    //! @param commandTag Expected command tag in packet
    virtual bool processResponse(const generic_response_packet_t *packet, uint8_t commandTag);

public:
    Progress *m_progress; //!< Variable for progress control.

protected:
    string_vector_t m_argv;           //!< Vector of argument strings.
    uint32_vector_t m_responseValues; //!< Vector of response values.
    string m_responseDetails;         //!< Descriptive response.
};

/*!
 * @brief Command packet operations.
 *
 * Formats command packets and runs the command phase.
 */
class CommandPacket
{
protected:
    //! @brief Constants.
    enum _command_packet_constants
    {
        kMaxCommandArguments = (kDefaultMaxPacketSize - sizeof(command_packet_t)) /
                               sizeof(uint32_t) //!< 7 args max for packet size 32 bytes
    };

    //! Format of command packet.
    struct PacketWithArgs
    {
        //! @brief Initialize the command packet.
        void init(uint8_t tag, uint8_t flags, uint8_t numArguments)
        {
            m_header.commandTag = tag;
            m_header.flags = flags;
            m_header.reserved = 0;
            m_header.parameterCount = numArguments;
        }
        command_packet_t m_header;                  //!< Packet header.
        uint32_t m_arguments[kMaxCommandArguments]; //!< Command arguments.
    };

public:
    //! @brief Constructor that takes no command arguments.
    CommandPacket(uint8_t tag, uint8_t flags = 0)
    {
        m_numArguments = 0;
        m_packet.init(tag, flags, m_numArguments);
    }

    //! @brief Constructor that takes one command argument.
    CommandPacket(uint8_t tag, uint8_t flags, uint32_t arg)
    {
        m_numArguments = 1;
        m_packet.init(tag, flags, m_numArguments);
        m_packet.m_arguments[0] = arg;
    }

    //! @brief Constructor that takes two command arguments.
    CommandPacket(uint8_t tag, uint8_t flags, uint32_t arg1, uint32_t arg2)
    {
        m_numArguments = 2;
        m_packet.init(tag, flags, m_numArguments);
        m_packet.m_arguments[0] = arg1;
        m_packet.m_arguments[1] = arg2;
    }

    //! @brief Constructor that takes three command arguments.
    CommandPacket(uint8_t tag, uint8_t flags, uint32_t arg1, uint32_t arg2, uint32_t arg3)
    {
        m_numArguments = 3;
        m_packet.init(tag, flags, m_numArguments);
        m_packet.m_arguments[0] = arg1;
        m_packet.m_arguments[1] = arg2;
        m_packet.m_arguments[2] = arg3;
    }

    //! @brief Constructor that takes four command arguments.
    CommandPacket(uint8_t tag, uint8_t flags, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
    {
        m_numArguments = 4;
        m_packet.init(tag, flags, m_numArguments);
        m_packet.m_arguments[0] = arg1;
        m_packet.m_arguments[1] = arg2;
        m_packet.m_arguments[2] = arg3;
        m_packet.m_arguments[3] = arg4;
    }

    //! @brief Constructor that takes four command arguments.
    CommandPacket(uint8_t tag, uint8_t flags, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5)
    {
        m_numArguments = 5;
        m_packet.init(tag, flags, m_numArguments);
        m_packet.m_arguments[0] = arg1;
        m_packet.m_arguments[1] = arg2;
        m_packet.m_arguments[2] = arg3;
        m_packet.m_arguments[3] = arg4;
        m_packet.m_arguments[4] = arg5;
    }

    //! @brief Get size of command packet, including arguments.
    uint32_t getSize() const { return sizeof(command_packet_t) + (m_numArguments * sizeof(uint32_t)); }
    //! @brief Get pointer to command packet data.
    const uint8_t *getData() { return reinterpret_cast<uint8_t *>(&m_packet.m_header); }
    //! @brief Send command packet and read response.
    //!
    //! @return Pointer to response packet.
    const uint8_t *sendCommandGetResponse(Packetizer &device);

protected:
    int m_numArguments;      //!< Number of command arguments.
    PacketWithArgs m_packet; //!< Command packet data.
};

/*!
 * @brief Data packet operations.
 *
 * Formats data packets and runs data phase.
 */
class DataPacket
{
public:
    /*!
     * @brief Abstract class to provide data for data phase.
     */
    class DataProducer
    {
    public:
        //! @brief Query if more data is available.
        virtual bool hasMoreData() const = 0;

        //! @brief Query the total size of the data.
        virtual uint32_t getDataSize() const = 0;

        //! @brief Get the next data chunk.
        //!
        //! Before calling getData(), call hasMoreData() to determine if
        //! data is available.
        virtual uint32_t getData(uint8_t *data, uint32_t size) = 0;
    };

    /*!
     * @brief Provide file data for data phase.
     */
    class FileDataProducer : public DataProducer
    {
    public:
        //! @brief Default constructor.
        FileDataProducer()
            : m_filePath()
            , m_filePointer(NULL)
            , m_fileSize(0)
        {
        }

        //! @brief Destructor.
        virtual ~FileDataProducer()
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

    protected:
        std::string m_filePath; //!< Data file path.
        FILE *m_filePointer;    //!< Data file pointer.
        long m_fileSize;        //!< Size in bytes of data file.
    };

    /*!
     * @brief Provide data from hex string for data phase.
     */
    class HexDataProducer : public DataProducer
    {
    public:
        //! @brief Default constructor.
        HexDataProducer()
            : m_byteIndex(0)
            , m_data()
        {
        }

        //! @brief Constructor that takes a vector<uchar> parameter.
        HexDataProducer(const uchar_vector_t &data)
            : m_byteIndex(0)
            , m_data(data)
        {
        }

        //! @brief Initialize with a data string.
        //!
        //! @param hexData String with hex digits surrounded by double brackets, e.g. '{{11 22 33}}', white space
        //! ignored
        //! @return Number of hex bytes parsed from the string.
        uint32_t initFromString(const std::string hexData);

        //! @brief Destructor.
        virtual ~HexDataProducer() {}
        //! \name DataProducer
        //@{
        //! @brief Query if more data is available.
        virtual bool hasMoreData() const { return (m_byteIndex < m_data.size()); }
        //! @brief Query the total size of the data.
        virtual uint32_t getDataSize() const { return m_data.size(); }
        //! @brief Get the next data chunk.
        //!
        //! Before calling getData(), call moreData() to determine if
        //! data is available.
        virtual uint32_t getData(uint8_t *data, uint32_t size);
        //@}

    protected:
        uint32_t m_byteIndex;  //!< Current byte index.
        uchar_vector_t m_data; //!< Data byte vector.
    };

    /*!
     * @brief Provide DataSource::Segment data for data phase.
     */
    class SegmentDataProducer : public DataProducer
    {
    public:
        //! @brief Default constructor.
        SegmentDataProducer(blfwk::DataSource::Segment *segment)
            : m_segment(segment)
            , m_byteIndex(0)
        {
        }

        //! @brief Destructor.
        virtual ~SegmentDataProducer() {}
        //! @brief Initialize with a file path.
        bool init(std::string filePath);

        //! \name DataProducer
        //@{
        //! @brief Query if more data is available.
        virtual bool hasMoreData() const { return (m_byteIndex < m_segment->getLength()); }
        //! @brief Query the total size of the data.
        virtual uint32_t getDataSize() const { return m_segment->getLength(); }
        //! @brief Get the next data chunk.
        //!
        //! Before calling getData(), call moreData() to determine if
        //! data is available.
        virtual uint32_t getData(uint8_t *data, uint32_t size);
        //@}

    protected:
        blfwk::DataSource::Segment *m_segment; //!< DataSource::Segment object.
        uint32_t m_byteIndex;                  //!< Current byte index.
    };

    /*!
     * @brief Abstract class to consume data from data phase.
     */
    class DataConsumer
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
    class FileDataConsumer : public DataConsumer
    {
    public:
        //! @brief Default constructor.
        FileDataConsumer()
            : m_filePath()
            , m_filePointer(NULL)
        {
        }

        //! @brief Destructor.
        virtual ~FileDataConsumer()
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

    /*!
     * @brief Print data for data phase receive.
     */
    class StdOutDataConsumer : public DataConsumer
    {
    public:
        enum _constants
        {
            kBytesPerLine = 16 //!< Number of hex bytes to display per line
        };

    public:
        //! @brief Constructor.
        StdOutDataConsumer()
            : m_currentCount(1)
            , m_dataCacheCount(0)
        {
        }

        //! @brief Finalize processing.
        virtual void finalize()
        {
            if (m_dataCacheCount)
            {
                for (int i = 0; i < m_dataCacheCount; ++i)
                {
                    printf("%02x", m_dataCache[i]);
                    if ((m_currentCount++ % kBytesPerLine) == 0)
                    {
                        printf("\n");
                    }
                    else
                    {
                        printf(" ");
                    }
                }
                m_dataCacheCount = 0;
            }
            if (((m_currentCount - 1) % kBytesPerLine) != 0)
            {
                // Fill space to clean the progress text.
                for (int i = ((m_currentCount - 1) % kBytesPerLine) * 3; i < 9; i++)
                {
                    printf(" ");
                }
                printf("\n");
            }
        }

        //! @brief Process the next data chunk.
        virtual void processData(const uint8_t *data, uint32_t size);

    protected:
        uint32_t m_currentCount;            //!< Current byte being processed, starts at 1
        uint8_t m_dataCache[kBytesPerLine]; //!< Cache the non-kBytesPerLine-aligned data.
        uint8_t m_dataCacheCount;           //!< Count in bytes of the cached data.
    };

public:
    //! @brief Constructor that takes a DataProducer.
    DataPacket(DataProducer *dataProducer, uint32_t packetSize = kDefaultMaxPacketSize)
    {
        assert(dataProducer);
        m_dataProducer = dataProducer;
        m_dataConsumer = NULL;
        m_packetSize = packetSize;
    }

    //! @brief Constructor that takes a DataConsumer.
    DataPacket(DataConsumer *dataConsumer)
    {
        assert(dataConsumer);
        m_dataConsumer = dataConsumer;
        m_dataProducer = NULL;
    }

    //! @brief Send data packet to device.
    //!
    //! Calls the data provide to get the data to send.
    uint8_t *sendTo(Packetizer &device, uint32_t *bytesWritten, Progress *progress);

    //! @brief Receive data packet from device.
    //!
    //! Calls the data consumer to process the receied data.
    uint8_t *receiveFrom(Packetizer &device, uint32_t *byteCount, Progress *progress);

protected:
    DataProducer *m_dataProducer;         //!< Provides data for the packet.
    DataConsumer *m_dataConsumer;         //!< Process the data in the packet.
    uint8_t m_packet[kMaxHostPacketSize]; //!< The data packet pointer.
    uint32_t m_packetSize;                //!< The data packet size of current device.
};

/*!
 * @brief Represents the bootloader Reset command.
 *
 * The Reset command has no arguments.
 */
class Reset : public Command
{
public:
    //! @brief Constructor that takes argument vector.
    Reset(const string_vector_t *argv)
        : Command(argv)
    {
    }

    //! @brief Default constructor.
    Reset()
        : Command(kCommand_Reset.name)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet), kCommandTag_Reset);
    }
};

/*!
 * @brief Represents the bootloader GetProperty command.
 */
class GetProperty : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    GetProperty(const string_vector_t *argv)
        : Command(argv)
        , m_property(kProperty_Invalid.value, kProperty_Invalid.description)
        , m_memoryIdorIndex(kMemoryInternal)
    {
    }

    //! @brief Constructor that takes a property_t argument.
    GetProperty(property_t property, uint32_t memoryIdorIndex = kMemoryInternal /*Default: internal memory or index 0*/)
        : Command(kCommand_GetProperty.name)
        , m_property(property)
        , m_memoryIdorIndex(memoryIdorIndex)
    {
        m_argv.push_back(format_string("0x%08x", property.value));
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const get_property_response_packet_t *packet);

    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_GetProperty);
    }

protected:
    property_t m_property;      //!< Property tag.
    uint32_t m_memoryIdorIndex; //!< External memory identifier
};

/*!
 * @brief Represents the bootloader GetProperty command.
 */
class SetProperty : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    SetProperty(const string_vector_t *argv)
        : Command(argv)
        , m_propertyTag(kProperty_Invalid.value)
        , m_propertyValue(0)
    {
    }

    //! @brief Constructor that takes a property_t argument and the value to set.
    SetProperty(property_t property, uint32_t value)
        : Command(kCommand_SetProperty.name)
        , m_propertyTag(property.value)
        , m_propertyValue(value)
    {
        m_argv.push_back(format_string("0x%08x", property.value));
        m_argv.push_back(format_string("0x%08x", value));
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_SetProperty);
    }

protected:
    uint32_t m_propertyTag;   //!< Property tag.
    uint32_t m_propertyValue; //!< Value to set.
};

/*!
 * @brief Represents the bootloader Flash Erase command.
 */
class FlashEraseRegion : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashEraseRegion(const string_vector_t *argv)
        : Command(argv)
        , m_startAddress(0)
        , m_byteCount(0)
        , m_memoryId(kMemoryInternal)
    {
    }

    //! @brief Constructor that takes a start and length arguments.
    FlashEraseRegion(uint32_t start, uint32_t length, uint32_t memoryId)
        : Command(kCommand_FlashEraseRegion.name)
        , m_startAddress(start)
        , m_byteCount(length)
        , m_memoryId(memoryId)
    {
        m_argv.push_back(format_string("0x%08x", start));
        m_argv.push_back(format_string("0x%08x", length));
        m_argv.push_back(format_string("%d", memoryId));
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FlashEraseRegion);
    }

protected:
    uint32_t m_startAddress; //!< Starting address in flash.
    uint32_t m_byteCount;    //!< Number of bytes to erase.
    uint32_t m_memoryId;     //!< Memory Device ID.
};

/*!
 * @brief Represents the bootloader Flash Erase All command.
 */
class FlashEraseAll : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashEraseAll(const string_vector_t *argv)
        : Command(argv)
        , m_memoryId(kMemoryInternal)
    {
    }

    //! @brief Constructor that takes memory ID argument.
    FlashEraseAll(uint32_t memoryId)
        : Command(kCommand_FlashEraseAll.name)
        , m_memoryId(memoryId)
    {
        m_argv.push_back(format_string("%d", memoryId));
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FlashEraseAll);
    }

protected:
    uint32_t m_memoryId; //!< Flash Memory Identifier
};

/*!
 * @brief Represents the bootloader Flash Erase All Unsecure command.
 */
class FlashEraseAllUnsecure : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashEraseAllUnsecure(const string_vector_t *argv)
        : Command(argv)
    {
    }

    //! @brief Constructor that takes none argument.
    FlashEraseAllUnsecure()
        : Command(kCommand_GetProperty.name)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FlashEraseAllUnsecure);
    }
};

/*!
* @brief Represents the bootloader Generate Key Blob command.
*/
class GenerateKeyBlob : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    GenerateKeyBlob(const string_vector_t *argv)
        : Command(argv)
        , m_fileDek()
        , m_fileKeyBlob()
        , m_count(0)
        , m_dataPhase(0)
    {
    }

    //! @brief Constructor
    GenerateKeyBlob()
        : Command(kCommand_GenerateKeyBlob.name)
        , m_fileDek()
        , m_fileKeyBlob()
        , m_count(0)
        , m_dataPhase(0)
    {
        m_argv.push_back(m_fileDek);
        m_argv.push_back(m_fileKeyBlob);
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const generate_key_blob_response_packet_t *packet);
    //! @brief Check response packet.

    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_GenerateKeyBlob);
    }

protected:
    std::string m_fileDek;     //!< DEK key Data file path.
    std::string m_fileKeyBlob; //!< Key Blob Data file path.
    uint32_t m_count;
    uint32_t m_dataPhase;
};

/*!
 * @brief Represents the bootloader Read Memory command.
 */
class ReadMemory : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ReadMemory(const string_vector_t *argv)
        : Command(argv)
        , m_startAddress(0)
        , m_byteCount(0)
        , m_dataFile()
        , m_memoryId(kMemoryInternal)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const read_memory_response_packet_t *packet);

    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_ReadMemory);
    }

protected:
    std::string m_dataFile;  //!< Data file path.
    uint32_t m_startAddress; //!< Destination memory address.
    uint32_t m_byteCount;    //!< Number of bytes to read.
    uint32_t m_memoryId;     //!< Memory device ID.
};

/*!
 * @brief Represents the bootloader Write Memory command.
 */
class WriteMemory : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    WriteMemory(const string_vector_t *argv)
        : Command(argv)
        , m_fileOrData()
        , m_segment(NULL)
        , m_startAddress(0)
        , m_count(0)
        , m_data()
        , m_memoryId(kMemoryInternal)
    {
    }

    //! @brief Constructor that takes an DataSource::Segment argument
    WriteMemory(blfwk::DataSource::Segment *segment, uint32_t memoryId)
        : Command(kCommand_WriteMemory.name)
        , m_fileOrData()
        , m_segment(segment)
        , m_count(0)
        , m_data()
        , m_memoryId(memoryId)
    {
        m_startAddress = segment->getBaseAddress();
        m_argv.push_back(format_string("0x%08x", m_startAddress));
        m_argv.push_back(m_fileOrData);
        m_argv.push_back(format_string("%d", memoryId));
    }

    //! @brief Constructor that takes an uchar_vector_t argument
    WriteMemory(uint32_t address, const uchar_vector_t &data, uint32_t memoryId)
        : Command(kCommand_WriteMemory.name)
        , m_fileOrData()
        , m_segment(NULL)
        , m_startAddress(address)
        , m_count(0)
        , m_data(data)
        , m_memoryId(memoryId)
    {
        m_argv.push_back(format_string("0x%08x", m_startAddress));
        m_argv.push_back(m_fileOrData);
        m_argv.push_back(format_string("%d", memoryId));
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_WriteMemory);
    }

protected:
    std::string m_fileOrData;              //!< Data file path or hex data string.
    blfwk::DataSource::Segment *m_segment; //!< DataSource segment (instead of file or hex string).
    uint32_t m_startAddress;               //!< Destination memory address.
    uint32_t m_count;                      //!< Number of bytes to write.
    uchar_vector_t m_data;                 //!< The data to write to the device.
    uint32_t m_memoryId;                   //!< Memory device ID
};

/*!
 * @brief Represents the bootloader Fill Memory command.
 */
class FillMemory : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FillMemory(const string_vector_t *argv)
        : Command(argv)
        , m_startAddress(0)
        , m_byteCount(0)
        , m_patternWord(0xFFFFFFFF)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FillMemory);
    }

protected:
    uint32_t m_startAddress; //!< Destination memory address.
    uint32_t m_byteCount;    //!< Number of bytes to fill.
    uint32_t m_patternWord;  //!< Fill pattern.
};

/*!
 * @brief Represents the bootloader Receive SB File command.
 */
class ReceiveSbFile : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ReceiveSbFile(const string_vector_t *argv)
        : Command(argv)
        , m_dataFile()
    {
    }

    //! @brief Constructor that takes a filename argument.
    ReceiveSbFile(const char *const filename)
        : Command(kCommand_ReceiveSbFile.name)
        , m_dataFile(filename)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_ReceiveSbFile);
    }

protected:
    std::string m_dataFile; //!< SB file path.
};

/*!
 * @brief Represents the bootloader Execute command.
 */
class Execute : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    Execute(const string_vector_t *argv)
        : Command(argv)
        , m_jumpAddress(0)
        , m_wordArgument(0)
        , m_stackpointer(0)
    {
    }

    //! @brief Constructor that takes entry_point, param and stack_pointer arguments.
    Execute(uint32_t entry_point, uint32_t param, uint32_t stack_pointer)
        : Command(kCommand_Execute.name)
        , m_jumpAddress(entry_point)
        , m_wordArgument(param)
        , m_stackpointer(stack_pointer)
    {
        m_argv.push_back(format_string("0x%08x", entry_point));
        m_argv.push_back(format_string("0x%08x", param));
        m_argv.push_back(format_string("0x%08x", stack_pointer));
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_Execute);
    }

protected:
    uint32_t m_jumpAddress;  //!< Destination memory address to jump to.
    uint32_t m_wordArgument; //!< Word argument passed to function.
    uint32_t m_stackpointer; //!< Stack pointer.
};

/*!
 * @brief Represents the bootloader Call command.
 */
class Call : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    Call(const string_vector_t *argv)
        : Command(argv)
        , m_callAddress(0)
        , m_wordArgument(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet), kCommandTag_Call);
    }

protected:
    uint32_t m_callAddress;  //!< Destination memory address to call.
    uint32_t m_wordArgument; //!< Word argument passed to function.
};

/*!
 * @brief Represents the bootloader Flash Security Disable command.
 */
class FlashSecurityDisable : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashSecurityDisable(const string_vector_t *argv)
        : Command(argv)
        , m_keyHigh(0)
        , m_keyLow(0)
    {
    }

    //! @brief Constructor that takes the key.
    FlashSecurityDisable(uint32_t keyHigh, uint32_t keyLow)
        : Command(kCommand_FlashSecurityDisable.name)
        , m_keyHigh(keyHigh)
        , m_keyLow(keyLow)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FlashSecurityDisable);
    }

protected:
    uint32_t m_keyLow;  //!< Bytes 0-3 of the flash backdoor key.
    uint32_t m_keyHigh; //!< Bytes 4-7 of the flash backdoor key.
};

/*!
 * @brief Represents the bootloader Flash Program Once command.
 */
class FlashProgramOnce : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashProgramOnce(const string_vector_t *argv)
        : Command(argv)
        , m_index(0)
        , m_byteCount(0)
        , m_dataLow(0)
        , m_dataHigh(0)
        , m_lsb(true)
    {
    }

    //! @brief Constructor that takes arguments.
    FlashProgramOnce(uint32_t index, uint32_t byteCount, uint32_t dataLow, uint32_t dataHigh, bool isLsb = true)
        : Command(kCommand_FlashProgramOnce.name)
        , m_index(index)
        , m_byteCount(byteCount)
        , m_dataLow(dataLow)
        , m_dataHigh(dataHigh)
        , m_lsb(isLsb)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FlashProgramOnce);
    }

protected:
    uint32_t m_index;     //!< Index of program once field to be programed.
    uint32_t m_byteCount; //!< Number of bytes to be programmed.
    uint32_t m_dataLow;   //!< Bytes 0-3 of the data to be written.
    uint32_t m_dataHigh;  //!< Bytes 4-7 of the data to be written.
    bool m_lsb;           //!< Sending sequence of the data.
};

/*!
 * @brief Represents the bootloader Flash Read Once command.
 */
class FlashReadOnce : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashReadOnce(const string_vector_t *argv)
        : Command(argv)
        , m_index(0)
        , m_byteCount(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const flash_read_once_response_packet_t *packet);

    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FlashReadOnce);
    }

protected:
    uint32_t m_index;     //!< Index of Program Once Field.
    uint32_t m_byteCount; //!< Number of bytes to be read.
};

/*!
 * @brief Represents the bootloader Read Resource command.
 */
class FlashReadResource : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashReadResource(const string_vector_t *argv)
        : Command(argv)
        , m_dataFile()
        , m_startAddress(0)
        , m_byteCount(0)
        , m_option(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const flash_read_resource_response_packet_t *packet);

    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_FlashReadResource);
    }

protected:
    std::string m_dataFile;  //!< Data file path.
    uint32_t m_startAddress; //!< Destination memory address.
    uint32_t m_byteCount;    //!< Number of bytes to read.
    uint32_t m_option;       //!< Resrouce to be read
};

/*!
 * @brief Represents the bootloader Configure QuadSpi command.
 */
class ConfigureMemory : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ConfigureMemory(const string_vector_t *argv)
        : Command(argv)
        , m_memoryId(kMemoryInternal)
        , m_configBlockAddress(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_ConfigureMemory);
    }

protected:
    uint32_t m_memoryId;           //!< Memory ID.
    uint32_t m_configBlockAddress; //!< Config block address (in RAM or in internal Flash).
};

/*!
* @brief Represents the bootloader Reliable Update command.
*/
class ReliableUpdate : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ReliableUpdate(const string_vector_t *argv)
        : Command(argv)
        , m_address(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_ReliableUpdate);
    }

protected:
    uint32_t m_address; //!< Destination address.
};

/*!
* @brief Represents the bootloader Key Provisioning command.
*/
class KeyProvisioning : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    KeyProvisioning(const string_vector_t *argv)
        : Command(argv)
        , m_operation(0)
        , m_type(0)
        , m_size(0)
        , m_memoryId(0)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

    void sendCmdAndData(Packetizer &device);

    void sendCmdAndGetData(Packetizer &device);

protected:
    //! @brief Check response packet.
    virtual bool processResponse(const key_provisioning_response_packet_t *packet);

    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_KeyProvisoning);
    }

protected:
    uint32_t m_operation; //!< Destination address.
    uint32_t m_type;
    uint32_t m_memoryId;
    uint32_t m_size;
    string m_fileOrData;
};

/*!
 * @brief Represents the Flash Image command used for downloading formatted file.
 */
class FlashImage : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    FlashImage(const string_vector_t *argv)
        : Command(argv)
        , m_fileName()
        , m_sourceFile(NULL)
        , m_doEraseOpt(false)
        , m_memoryId(kMemoryInternal)
    {
    }

    //! @brief Constructor that takes a SourceFile argument
    FlashImage(SourceFile *sourceFile, bool doEraseOpt, uint32_t memoryId)
        : Command(kCommand_FlashImage.name)
        , m_fileName()
        , m_sourceFile(sourceFile)
        , m_doEraseOpt(doEraseOpt)
        , m_memoryId(memoryId)
    {
        m_argv.push_back(m_sourceFile->getPath());
        m_argv.push_back(doEraseOpt ? "erase" : "none");
        m_argv.push_back(format_string("%d", memoryId));
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    std::string m_fileName;   //!< Image file name with full path.
    SourceFile *m_sourceFile; //!< Sourcefile object containing the data and addresses.
    bool m_doEraseOpt;        //!< Detemine if doing erase operation before writting image file to flash.
    uint32_t m_memoryId;      //!< Memory device ID.
};

/*!
 * @brief Represents the bootloader Configure I2C command.
 */
class ConfigureI2c : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ConfigureI2c(const string_vector_t *argv)
        : Command(argv)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_ConfigureI2c);
    }

protected:
    uint8_t i2cAddress = 0x10;
    uint32_t i2cSpeedKHz = 100;
};

/*!
 * @brief Represents the bootloader Configure SPI command.
 */
class ConfigureSpi : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ConfigureSpi(const string_vector_t *argv)
        : Command(argv)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_ConfigureSpi);
    }

protected:
    uint32_t spiSpeedKHz = 100;
    BusPal::spi_clock_polarity_t spiPolarity = BusPal::kSpiClockPolarity_ActiveLow;
    BusPal::spi_clock_phase_t spiPhase = BusPal::kSpiClockPhase_SecondEdge;
    BusPal::spi_shift_direction_t spiDirection = BusPal::kSpiMsbFirst;
};

/*!
 * @brief Represents the bootloader Configure CAN command.
 */
class ConfigureCan : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ConfigureCan(const string_vector_t *argv)
        : Command(argv)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    //! @brief Check generic response packet.
    virtual bool processResponse(const uint8_t *packet)
    {
        return Command::processResponse(reinterpret_cast<const generic_response_packet_t *>(packet),
                                        kCommandTag_ConfigureCan);
    }

protected:
    uint32_t canSpeed = 4;
    uint32_t canTxid = 0x321;
    uint32_t canRxid = 0x123;
};

/*!
* @brief Represents the ListMemory command used to list all memories available on current target device
*/
class ListMemory : public Command
{
public:
    //! @brief Constructor that takes an argument vector.
    ListMemory(const string_vector_t *argv)
        : Command(argv)
    {
    }

    //! @brief Constructor that takes a SourceFile argument
    ListMemory(void)
        : Command(kCommand_FlashImage.name)
    {
    }

    //! @brief Initialize.
    virtual bool init();

    //! @brief Send command to packetizer.
    virtual void sendTo(Packetizer &packetizer);

protected:
    // No processResponse here.
    // Command sequence doesn't need to process response.
};

//@}

} // namespace blfwk

//! @}

#endif // _Command_h_

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
