/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cstring>
#include "blfwk/Bootloader.h"
#include "blfwk/UsbHidPacketizer.h"
#include "blfwk/SerialPacketizer.h"
#include "blfwk/options.h"
#include "blfwk/utils.h"

#if defined(WIN32)
#include "windows.h"
#elif defined(LINUX)
#include "signal.h"
#endif

using namespace blfwk;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief The tool's name.
const char k_toolName[] = "blhost";

//! @brief Current version number for the tool.
const char k_version[] = "2.2.0";

//! @brief Copyright string.
const char k_copyright[] = "Copyright (c) 2013-2015 Freescale Semiconductor, Inc.\n\
Copyright 2016-2018 NXP\nAll rights reserved.";

//! @brief Command line option definitions.
static const char *k_optionsDefinition[] = { "?|help", "v|version", "p:port <name>[,<speed>]",
                                             "b:buspal spi[,<speed>,<polarity>,<phase>,lsb|msb] | "
                                             "i2c[,<address>,<speed>] | can[,<speed>,<txid>,<rxid>]",
                                             "u?usb [[<vid>,]<pid>] | [<path>]", "V|verbose", "d|debug", "j|json", "n|noping",
                                             "t:timeout <ms>", NULL };

//! @brief Usage text.
const char k_optionUsage[] =
    "\nOptions:\n\
  -?/--help                    Show this help\n\
  -v/--version                 Display tool version\n\
  -p/--port <name>[,<speed>]   Connect to target over UART. Specify COM port\n\
                               and optionally baud rate\n\
                                 (default=57600)\n\
                                 If -b, then port is BusPal port\n\
  -b/--buspal spi[,<speed>,<polarity>,<phase>,lsb|msb] |\n\
              i2c[,<address>,<speed>]\n\
              can[,<speed>,<txid>,<rxid>]\n\
                               Use SPI or I2C for BusPal<-->Target link\n\
                               All parameters between square brackets are\n\
                               optional, but preceding parameters must be\n\
                               present or marked with a comma.\n\
                               (ex. -b spi,1000,0,1) (ex. -b spi,1000,,lsb)\n\
                                 spi:  speed(KHz),\n\
                                       polarity(0=active_high | 1=active_low),\n\
                                       phase(0=rising_edge | 1=falling_edge),\n\
                                       \"lsb\" | \"msb\"\n\
                                       (default=100,1,1,msb)\n\
                                 i2c:  address(7-bit hex), speed(KHz)\n\
                                       (default=0x10,100)\n\
                                 can:  speed(0=125K | 1=250K | 2=500K | 4=1M),\n\
                                       txid (11 bits ID),\n\
                                       rxid (11 bits ID)\n\
                                       (default=4,0x321,0x123)\n\
  -u/--usb [[[<vid>,]<pid>] | [<path>]]\n\
                               Connect to target over USB HID device denoted by\n\
                               vid/pid (default=0x15a2,0x0073) or device path\n\
  -V/--verbose                 Print extra detailed log information\n\
  -d/--debug                   Print really detailed log information\n\
  -j/--json                    Print output in JSON format to aid automation.\n\
  -n/--noping                  Skip the initial ping of a serial target\n\
  -t/--timeout <ms>            Set packet timeout in milliseconds\n\
                                 (default=5000)\n";

//! @brief Trailer usage text that gets appended after the options descriptions.
static const char *usageTrailer = "-- command <args...>";

//! @brief Memory id list.
const char k_memoryId[] =
    "\nMemory ID:\n\
  Internal Memory              Device internal memory space\n\
    0                            Internal Memory\n\
                                 (Default selected memory)\n\
    16 (0x10)                    Execute-only region on internal flash\n\
                                 (Only used for flash-erase-all)\n\
  Mapped External Memory       The memories that are remapped to internal space,\n\
                               and must be accessed by internal addresses.\n\
                               (IDs in this group are only used for flash-erase-all and\n\
                               configure-memory, and ignored by write-memory, read-memory,\n\
                               flash-erase-region and flash-image(use default 0))\n\
    1                            QuadSPI Memory\n\
    8                            SEMC NOR Memory\n\
    9                            FlexSPI NOR Memory\n\
    10 (0xa)                     SPIFI NOR Memory\n\
  Unmapped External Memory     Memories which cannot be remapped to internal space,\n\
                               and only can be accessed by memories' addresses.\n\
                               (Must be specified for all commands with <memoryId> argument)\n\
    256 (0x100)                  SEMC NAND Memory\n\
    257 (0x101)                  SPI NAND Memory\n\
    272 (0x110)                  SPI NOR/EEPROM Memory\n\
    273 (0x111)                  I2C NOR/EEPROM Memory\n\
    288 (0x120)                  uSDHC SD Memory\n\
    289 (0x121)                  uSDHC MMC Memory\n\
\n\
** Note that not all memories are supported on all platforms.\n";

//! @brief Command usage string.
const char k_commandUsage[] =
    "\nCommand:\n\
  reset                        Reset the chip\n\
  get-property <tag> [<memoryId> | <index>]\n\
    1                          Bootloader version\n\
    2                          Available peripherals\n\
    3                          Start of program flash\n\
    4                          Size of program flash\n\
    5                          Size of flash sector\n\
    6                          Blocks in flash array\n\
    7                          Available commands\n\
    8                          CRC check status\n\
    10                         Verify Writes flag\n\
    11                         Max supported packet size\n\
    12                         Reserved regions\n\
    14                         Start of RAM, <index> is required\n\
    15                         Size of RAM, <index> is required\n\
    16                         System device identification\n\
    17                         Flash security state\n\
    18                         Unique device identification\n\
    19                         FAC support flag\n\
    20                         FAC segment size\n\
    21                         FAC segment count\n\
    22                         Read margin level of program flash\n\
    23                         QuadSpi initialization status\n\
    24                         Target version\n\
    25                         External Memory Attrubutes, <memoryId> is required.\n\
  set-property <tag> <value>\n\
    10                         Verify Writes flag\n\
    22                         Read margin level of program flash\n\
  flash-erase-region <addr> <byte_count> [memory_id]\n\
                               Erase a region of flash according to [memory_id].\n\
  flash-erase-all [memory_id]  Erase all flash according to [memory_id],\n\
                               excluding protected regions.\n\
  flash-erase-all-unsecure     Erase all internal flash, including protected regions\n\
  read-memory <addr> <byte_count> [<file>] [memory_id]\n\
                               Read memory according to [memory_id] and write to file\n\
                               or stdout if no file specified\n\
  write-memory <addr> [<file>[,byte_count]| {{<hex-data>}}] [memory_id]\n\
                               Write memory according to [memory_id] from file\n\
                               or string of hex values,\n\
                               e.g. data.bin (writes entire file)\n\
                               e.g. data.bin 8 (writes first 8 bytes from file)\n\
                               e.g. \"{{11 22 33 44}}\" (w/quotes)\n\
                               e.g. {{11223344}} (no spaces)\n\
  fill-memory <addr> <byte_count> <pattern> [word | short | byte]\n\
                               Fill memory with pattern; size is\n\
                               word (default), short or byte\n\
  receive-sb-file <file>       Receive SB file\n\
  execute <addr> <arg> <stackpointer>\n\
                               Execute at address with arg and stack pointer\n\
  call <addr> <arg>            Call address with arg\n\
  flash-security-disable <key> Flash Security Disable <8-byte-hex-key>,\n\
                               e.g. 0102030405060708\n\
  flash-program-once <index> <byte_count> <data> [LSB | MSB]\n\
                               Program Flash Program Once Field \n\
                               <data> is 4 or 8-byte-hex according to <byte_count>\n\
                               <data> output sequence is specified by LSB(Default) or MSB\n\
                               The output sequence of data \"1234\" is \"4,3,2,1\" by default,\n\
                               while is \"1,2,3,4\" if MSB is specified\n\
                               e.g. 0 4 12345678 MSB\n\
  flash-read-once <index> <byte_count>  \n\
                               Read Flash Program Once Field\n\
  flash-read-resource <addr> <byte_count> <option> [<file>] \n\
                               Read Resource from special-purpose\n\
                               non-volatile memory and write to file\n\
                               or stdout if no file specified\n\
  configure-memory <memory_id> <internal_addr>\n\
                               Apply configuration block at internal memory address\n\
                               <internal_addr> to memory with ID <memory_id>\n\
  reliable-update <addr>\n\
                               Copy backup app from address to main app region\n\
                               or swap flash using indicator address\n\
  key-provisioning <operation> [arguments...]\n\
                               <enroll>\n\
                                   Key provisioning enroll. No argument for this operation\n\
                               <set_user_key> <type> <file>[,<size>]\n\
                                   Send the user key specified by <type> to bootloader. <file> is\n\
                                   the binary file containing user key plaintext. If <size> is not\n\
                                   specified, the entire <file> will be sent. Otherwise, only send\n\
                                   the first <size> bytes\n\
                               <set_key> <type> <size>\n\
                                   Generate <size> bytes of the key specified by <type>\n\
                               <write_key_nonvolatile> [memoryID]\n\
                                   Write the key to a nonvolatile memory\n\
                               <read_key_nonvolatile> [memoryID]\n\
                                   Load the key from a nonvolatile memory to bootloader\n\
                               <write_key_store> <file>[,<size>]\n\
                                   Send the key store to bootloader. <file> is the binary file\n\
                                   containing key store. If <size> is not specified, the entire\n\
                                   <file> will be sent. Otherwise, only send the first <size> bytes\n\
                               <read_key_store> <file>\n\
                                   Read the key store from bootloader to host(PC). <file> is the\n\
                                   binary file to store the key store\n\
  flash-image <file> [erase] [memory_id]\n\
                               Write a formated image <file> to memory with ID\n\
                               <memory_id>. Supported file types: SRecord\n\
                               (.srec and .s19) and HEX (.hex). Flash is erased\n\
                               before writing if [erase]=erase. The erase unit\n\
                               size depends on the target and the minimum erase\n\
                               unit size is 1K.\n\
  list-memory                  List all on-chip Flash and RAM regions, and off-chip\n\
                               memories, supported by current device.\n\
                               Only the configured off-chip memory will be list.\n\
  efuse-program-once <addr> <data> [nolock/lock]\n\
                               Program one word of OCOTP Field \n\
                               <addr> is ADDR of OTP word, not the shadowed memory address.\n\
                               <data> is hex digits without prefix '0x'\n\
  efuse-read-once <addr>\n\
                               Read one word of OCOTP Field\n\
                               <addr> is ADDR of OTP word, not the shadowed memory address.\n\
  program-aeskey <file>\n\
                               Program AES key to OTP Field\n\
                               <file> is a raw binary contains an 128-bits key.\n\
  generate-key-blob <dek_file> <blob_file> \n\
                               Generate the Blob for given Dek Key\n\
                               <dek_file> - input, a binary Dek Key (128 Bits) generated by CST tool.\n\
                               <blob_file> - output, a generated blob (72 Bytes) in binary format.\n\
\n\
** Note that not all commands/properties are supported on all platforms.\n";

/*!
 * \brief Class that encapsulates the blhost tool.
 *
 * A single global logger instance is created during object construction. It is
 * never freed because we need it up to the last possible minute, when an
 * exception could be thrown.
 */
class BlHost
{
public:
    /*!
     * Constructor.
     *
     * Creates the singleton logger instance.
     */
    BlHost(int argc, char *argv[])
        : m_argc(argc)
        , m_argv(argv)
        , m_cmdv()
        , m_comPort("COM1")
        , m_comSpeed(57600)
        , m_useBusPal(false)
        , m_busPalConfig()
        , m_logger(NULL)
        , m_useUsb(false)
        , m_useUart(false)
        , m_usbVid(UsbHidPeripheral::kDefault_Vid)
        , m_usbPid(UsbHidPeripheral::kDefault_Pid)
        , m_packetTimeoutMs(5000)
        , m_ping(true)
    {
        // create logger instance
        m_logger = new StdoutLogger();
        m_logger->setFilterLevel(Logger::kInfo);
        Log::setLogger(m_logger);

#if defined(WIN32)
        // set ctrl handler for Ctrl + C and Ctrl + Break signal
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlHandler, TRUE);
#elif defined(LINUX)
        // set ctrl handler for Ctrl + C signal, Ctrl + Break doesnot take effect under LINUX system.
        signal(SIGINT, ctrlPlusCHandler);
#endif
    }

    //! @brief Destructor.
    virtual ~BlHost() {}
    //! @brief Run the application.
    int run();

protected:
    //! @brief Process command line options.
    int processOptions();
//! @brief Handler for Ctrl signals
#if defined(WIN32)
    static BOOL ctrlHandler(DWORD ctrlType);
#elif defined(LINUX)
    static void ctrlPlusCHandler(int msg);
#endif
    static void displayProgress(int percentage, int segmentIndex, int segmentCount);

protected:
    int m_argc;                     //!< Number of command line arguments.
    char **m_argv;                  //!< Command line arguments.
    string_vector_t m_cmdv;         //!< Command line argument vector.
    string m_comPort;               //!< COM port to use.
    int m_comSpeed;                 //!< COM port speed.
    bool m_useBusPal;               //!< True if using BusPal peripheral.
    string_vector_t m_busPalConfig; //!< Bus pal peripheral-specific argument vector.
    bool m_useUsb;                  //!< Connect over USB HID.
    bool m_useUart;                 //!< Connect over UART.
    uint16_t m_usbVid;              //!< USB VID of the target HID device
    uint16_t m_usbPid;              //!< USB PID of the target HID device
    string m_usbPath;               //!< USB PATH of the target HID device
    bool m_ping;                    //!< If true will not send the initial ping to a serial device
    uint32_t m_packetTimeoutMs;     //!< Packet timeout in milliseconds.
    ping_response_t m_pingResponse; //!< Response to initial ping
    StdoutLogger *m_logger;         //!< Singleton logger instance.
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

//! @brief Print command line usage.
static void printUsage()
{
    printf(k_optionUsage);
    printf(k_memoryId);
    printf(k_commandUsage);
}

#if defined(WIN32)
BOOL BlHost::ctrlHandler(DWORD ctrlType)
{
    switch (ctrlType)
    {
        // Trap both of Ctrl + C and Ctrl + Break signal.
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
            Log::warning(
                "\nWarning: Operation canceled!\n- The target device must be reset before sending any further "
                "commands.\n");
            return false;
        default:
            return false;
    }
}
#elif defined(LINUX)
void BlHost::ctrlPlusCHandler(int msg)
{
    if (msg == SIGINT)
    {
        Log::warning(
            "\nWarning: Operation canceled!\n- The target device must be reset before sending any further commands.\n");
        exit(0);
    }
}
#endif

void BlHost::displayProgress(int percentage, int segmentIndex, int segmentCount)
{
    Log::info("\r(%d/%d)%2d%%", segmentIndex, segmentCount, percentage);
    if (percentage >= 100)
    {
        Log::info(" Completed!\n");
    }
}

int BlHost::processOptions()
{
    Options options(*m_argv, k_optionsDefinition);

    // If there are no arguments print the usage.
    if (m_argc == 1)
    {
        options.usage(std::cout, usageTrailer);
        printUsage();
        return 0;
    }

    OptArgvIter iter(--m_argc, ++m_argv);

    // Process command line options.
    int optchar;
    const char *optarg;
    while ((optchar = options(iter, optarg)))
    {
        switch (optchar)
        {
            case '?':
                options.usage(std::cout, usageTrailer);
                printUsage();
                return 0;

            case 'v':
                printf("%s %s\n%s\n", k_toolName, k_version, k_copyright);
                return 0;
                break;

            case 'p':
            {
                if (m_useUsb)
                {
                    Log::error("Error: You cannot specify both -u and -p options.\n");
                    options.usage(std::cout, usageTrailer);
                    printUsage();
                    return 0;
                }
#if defined(WIN32)
                if (optarg && (string(optarg)[0] == 'c' || string(optarg)[0] == 'C'))
#else
                if (optarg)
#endif
                {
                    string_vector_t params = utils::string_split(optarg, ',');
                    m_comPort = params[0];
                    if (params.size() == 2)
                    {
                        int speed = atoi(params[1].c_str());
                        if (speed <= 0)
                        {
                            Log::error("Error: You must specify a valid baud rate with the -p/--port option.\n");
                            options.usage(std::cout, usageTrailer);
                            printUsage();
                            return 0;
                        }
                        m_comSpeed = speed;
                    }
                }
                else
                {
                    Log::error("Error: You must specify the COM port identifier string with the -p/--port option.\n");
                    options.usage(std::cout, usageTrailer);
                    printUsage();
                    return 0;
                }
                m_useUart = true;
                break;
            }
            case 'b':
            {
                if (optarg)
                {
                    m_busPalConfig = utils::string_split(optarg, ',');
                    if ((strcmp(m_busPalConfig[0].c_str(), "spi") != 0) &&
                        (strcmp(m_busPalConfig[0].c_str(), "i2c") != 0) &&
                        (strcmp(m_busPalConfig[0].c_str(), "can") != 0) &&
                        (strcmp(m_busPalConfig[0].c_str(), "gpio") != 0) &&
                        (strcmp(m_busPalConfig[0].c_str(), "clock") != 0))
                    {
                        Log::error("Error: %s is not valid for option -b/--buspal.\n", m_busPalConfig[0].c_str());
                        options.usage(std::cout, usageTrailer);
                        printUsage();
                        return 0;
                    }

                    m_useBusPal = true;
                }
                break;
            }
            case 'u':
            {
                if (m_useUart)
                {
                    Log::error("Error: You cannot specify both -u and -p options.\n");
                    options.usage(std::cout, usageTrailer);
                    printUsage();
                    return 0;
                }
                if (optarg)
                {
                    string_vector_t params = utils::string_split(optarg, ',');
                    uint32_t tempId = 0;
                    bool useDefaultVid = true;
                    bool useDefaultPid = true;
                    uint16_t vid = 0, pid = 0;
                    string path;
                    if (params.size() == 1)
                    {
                        if (utils::stringtoui(params[0].c_str(), tempId) && tempId < 0x00010000)
                        {
                            pid = (uint16_t)tempId;
                            useDefaultPid = false;
                        }
                        else
                        {
                            path = params[0];
                        }
                    }
                    else if (params.size() == 2)
                    {
                        if (utils::stringtoui(params[0].c_str(), tempId) && tempId < 0x00010000)
                        {
                            vid = (uint16_t)tempId;
                            useDefaultVid = false;
                        }
                        else
                        {
                            Log::error("Error: %s is not valid vid for option -u.\n", params[0].c_str());
                            return 0;
                        }
                        if (utils::stringtoui(params[1].c_str(), tempId) && tempId < 0x00010000)
                        {
                            pid = (uint16_t)tempId;
                            useDefaultPid = false;
                        }
                        else
                        {
                            Log::error("Error: %s is not valid pid for option -u.\n", params[1].c_str());
                            return 0;
                        }
                    }
                    if (!useDefaultPid)
                    {
                        m_usbPid = pid;
                    }
                    if (!useDefaultVid)
                    {
                        m_usbVid = vid;
                    }
                    m_usbPath = path;
                }
                m_useUsb = true;
                break;
            }

            case 'V':
                Log::getLogger()->setFilterLevel(Logger::kDebug);
                break;

            case 'd':
                Log::getLogger()->setFilterLevel(Logger::kDebug2);
                break;

            case 'j':
                Log::getLogger()->setFilterLevel(Logger::kJson);
                break;

            case 'n':
                m_ping = false;
                break;

            case 't':
                if (optarg)
                {
                    uint32_t timeout = 0;
                    if (utils::stringtoui(optarg, timeout))
                    {
                        m_packetTimeoutMs = timeout;
                    }
                    else
                    {
                        Log::error("Error: %s is not valid for option -t/--timeout.\n", optarg);
                        options.usage(std::cout, usageTrailer);
                        printUsage();
                        return 0;
                    }
                }
                break;

            // All other cases are errors.
            default:
                return 1;
        }
    }

    // Treat the rest of the command line as a single bootloader command,
    // possibly with arguments. Allow bus pal to be used without a command argument
    // for things like GPIO configuration
    if (iter.index() == m_argc && !m_useBusPal)
    {
        options.usage(std::cout, usageTrailer);
        printUsage();
        return 0;
    }

    // Save command name and arguments.
    for (int i = iter.index(); i < m_argc; ++i)
    {
        m_cmdv.push_back(m_argv[i]);
    }

    // All is well.
    return -1;
}

int BlHost::run()
{
    status_t result = kStatus_Success;
    Peripheral::PeripheralConfigData config;
    Command *cmd = NULL;
    Command *configCmd = NULL;
    Progress *progress = NULL;
    // Read command line options.
    int optionsResult;
    if ((optionsResult = processOptions()) != -1)
    {
        return optionsResult;
    }

    try
    {
        if (m_cmdv.size())
        {
            // Check for any passed commands and validate command.
            cmd = Command::create(&m_cmdv);
            if (!cmd)
            {
                std::string msg = format_string("Error: invalid command or arguments '%s", m_cmdv.at(0).c_str());
                string_vector_t::iterator it = m_cmdv.begin();
                for (++it; it != m_cmdv.end(); ++it)
                {
                    msg.append(format_string(" %s", (*it).c_str()));
                }
                msg.append("'\n");
                throw std::runtime_error(msg);
            }

            progress = new Progress(displayProgress, NULL);
            cmd->registerProgress(progress);
        }

        config.ping = m_ping;

        if (m_useUsb)
        {
            config.peripheralType = Peripheral::kHostPeripheralType_USB_HID;
            config.usbHidVid = m_usbVid;
            config.usbHidPid = m_usbPid;
            config.usbPath = m_usbPath;
            config.packetTimeoutMs = m_packetTimeoutMs;
            if (m_useBusPal)
            {
                if (!BusPal::parse(m_busPalConfig, config.busPalConfig))
                {
                    std::string msg =
                        format_string("Error: %s is not valid for option -b/--buspal.\n", m_busPalConfig[0].c_str());
                    throw std::runtime_error(msg);
                }

                // Check for any passed commands and validate command.
                configCmd = Command::create(&m_busPalConfig);
                if (!configCmd)
                {
                    std::string msg =
                        format_string("Error: invalid command or arguments '%s", m_busPalConfig.at(0).c_str());
                    string_vector_t::iterator it = m_busPalConfig.begin();
                    for (++it; it != m_busPalConfig.end(); ++it)
                    {
                        msg.append(format_string(" %s", (*it).c_str()));
                    }
                    msg.append("'\n");
                    throw std::runtime_error(msg);
                }
            }
        }
        else
        {
            config.peripheralType = Peripheral::kHostPeripheralType_UART;
            config.comPortName = m_comPort.c_str();
            config.comPortSpeed = m_comSpeed;
            config.packetTimeoutMs = m_packetTimeoutMs;
            if (m_useBusPal)
            {
                config.peripheralType = Peripheral::kHostPeripheralType_BUSPAL_UART;
                if (!BusPal::parse(m_busPalConfig, config.busPalConfig))
                {
                    std::string msg =
                        format_string("Error: %s is not valid for option -b/--buspal.\n", m_busPalConfig[0].c_str());
                    throw std::runtime_error(msg);
                }
            }
        }

        // Init the Bootloader object.
        Bootloader *bl = new Bootloader(config);

        if (configCmd)
        {
            // If we have a command inject it.
            Log::info("Inject pre-config command '%s'\n", configCmd->getName().c_str());
            bl->inject(*configCmd);
            bl->flush();

            if (configCmd->getResponseValues()->size() > 0)
            {
                if (configCmd->getResponseValues()->at(0) != kStatus_NoResponseExpected)
                {
                    // Print command response values.
                    configCmd->logResponses();
                }

                // Only thing we consider an error is NoResponse
                if (configCmd->getResponseValues()->at(0) == kStatus_NoResponse)
                {
                    result = kStatus_NoResponse;
                }
            }

            delete configCmd;
        }

        if (cmd)
        {
            // If we have a command inject it.
            Log::info("Inject command '%s'\n", cmd->getName().c_str());
            bl->inject(*cmd);
            bl->flush();

            if (cmd->getResponseValues()->size() > 0)
            {
                if (cmd->getResponseValues()->at(0) != kStatus_NoResponseExpected)
                {
                    // Print command response values.
                    cmd->logResponses();
                }

                // Only thing we consider an error is NoResponse
                if (cmd->getResponseValues()->at(0) == kStatus_NoResponse)
                {
                    result = kStatus_NoResponse;
                }
            }

            delete cmd;
        }

        delete bl;
    }
    catch (exception &e)
    {
        Log::error(e.what());
        result = kStatus_Fail;
    }

    if (progress)
    {
        delete progress;
    }

    return result;
}

//! @brief Application entry point.
int main(int argc, char *argv[], char *envp[])
{
    return BlHost(argc, argv).run();
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
