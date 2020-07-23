/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include <iostream>
#if WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdexcept>
#include "ConversionController.h"
#include "options.h"
#include "Version.h"
#include "EncoreBootImage.h"
#include "SB2Image.h"
#include "iMXImage.h"
#include "smart_ptr.h"
#include "Logging.h"
#include "EncoreBootImageGenerator.h"
#include "SB2ImageGenerator.h"
#include "iMXImageGenerator.h"
#include "EncoreBootImageReader.h"
#include "SearchPath.h"
#include "format_string.h"
#include "AESKey.h"
#include "time.h"

using namespace elftosb;

//! An array of strings.
// typedef std::vector<std::string> string_vector_t; defined in sb2image.h

//! The tool's name.
const char k_toolName[] = "elftosb";

//! Current version number for the tool.
const char k_version[] = "4.0.0";

//! Copyright string.
const char k_copyright[] = "Copyright (c) 2004-2015 Freescale Semiconductor, Inc.\n\
Copyright 2016-2018 NXP\nAll rights reserved.";

static const char *k_optionsDefinition[] = { "?|help",
                                             "v|version",
                                             "f:chip-family <family>",
                                             "c:command <file>",
                                             "o:output <file>",
                                             "P:product <version>",
                                             "C:component <version>",
                                             "k:key <file>",
                                             "z|zero-key",
                                             "D:define <const>",
                                             "O:option <option>",
                                             "K?keygen <option>",
                                             "n:number <int>",
                                             "x|extract",
                                             "x|sbtool",
                                             "i:index <int>",
                                             "b|binary",
                                             "s:pkey <file>",
                                             "S:cert <file>",
                                             "R:root key cert <file>",
                                             "h:output hash of hashes <file>",
                                             "d|debug",
                                             "q|quiet",
                                             "V|verbose",
                                             "p:search-path <path>",
                                             NULL };

//! Help string.
const char k_usageText[] =
    "\nOptions:\n\
  -?/--help                    Show this help\n\
  -v/--version                 Display tool version\n\
  -f/--chip-family <family>    Select the chip family (default is kinetisK3)\n\
  -c/--command <file>          Use this command file\n\
  -o/--output <file>           Write output to this file\n\
  -p/--search-path <path>      Add a search path used to find input files\n\
  -P/--product <version        Set product version\n\
  -C/--component <version>     Set component version\n\
  -k/--key <file>              Add OTP key, used for encryption and decryption\n\
                               The first -k provides the kek\n\
                               All other occurances of -k on cmd line will be ignored\n\
  -z/--zero-key                Add default key of all zeroes\n\
  -D/--define <const>=<int>    Define or override a constant value\n\
  -O/--option <name>=<value>   Set or override a processing option\n\
  -K/--keygen <option>         Generate AES-128 or AES-256 key file [128, 256], default 128\n\
  -n/--number <int>            Number of keys to generate per file\n\
                               (default=1)(valid only when -K is specified)\n\
  -x/--extract/--sbtool        Extract a specified section\n\
  -i/--index <int>             Section index to extract(default=None Section)\n\
                               (valid only when -x is specified)\n\
  -b/--binary                  Extract section data as binary\n\
                               (valid only when -x is specified)\n\
                               (Warning: -q is enabled implicitly if -b is specified)\n\
  -s/--pkey <file>             Path to private key for signing\n\
  -S/--cert <file>             Path to cerificate files for signing,\n\
                               the first certificate will be the self signed root key certificate\n\
  -R/--root-key-cert <file>    Path to root key cerificate file(s) for verifying other certificates,\n\
                               max of 4 root key certs are allowed and the rest are ignored,\n\
                               one of the certificate must match the first certificate passed with -S/--cert arg\n\
  -h/--hash-of-hashes <file>   Path to output hash of hashes of root keys,\n\
                               if argument is not provided then by default the tool creates hash.bin in the working directory,\n\
  -d/--debug                   Enable debug output\n\
  -q/--quiet                   Output only warnings and errors\n\
  -V/--verbose                 Print extra detailed log information\n\n";

// prototypes
int main(int argc, char *argv[], char *envp[]);

/*!
 * \brief Class that encapsulates the elftosb tool.
 *
 * A single global logger instance is created during object construction. It is
 * never freed because we need it up to the last possible minute, when an
 * exception could be thrown.
 */
class elftosbTool
{
protected:
    //! Supported chip families.
    enum chip_family_t
    {
        k37xxFamily,      //!< 37xx series.
        kMX28Family,      //!< Catskills series.
        kKinetisFamily,   //!< Kinetis devices.
        kKinetisK3Family, //!< Kinetis K3S and up family of devices.
        kiMXFamily,       //!< i.MX family of devices including RTs and Ultra
    };

	/*!
     * \brief A structure describing an entry in the table of chip family names.
     */
    struct FamilyNameTableEntry
    {
        const char *const name;
        chip_family_t family;
    };

    //! \brief Table that maps from family name strings to chip family constants.
    static const FamilyNameTableEntry kFamilyNameTable[];

    int m_argc;                                 //!< Number of command line arguments.
    char **m_argv;                              //!< String value for each command line argument.
    StdoutLogger *m_logger;                     //!< Singleton logger instance.
    string_vector_t m_keyFilePaths;             //!< Paths to OTP key files.
    string_vector_t m_positionalArgs;           //!< Arguments coming after explicit options.
    bool m_isVerbose;                           //!< Whether the verbose flag was turned on.
    bool m_useDefaultKey;                       //!< Include a default (zero) crypto key.
    const char *m_commandFilePath;              //!< Path to the elftosb command file.
    const char *m_outputFilePath;               //!< Path to the output .sb file.
    const char *m_hashFilePath;                 //!< Path to the output hash binary file.
    const char *m_searchPath;                   //!< Optional search path for input files.
    elftosb::version_t m_productVersion;        //!< Product version specified on command line.
    elftosb::version_t m_componentVersion;      //!< Component version specified on command line.
    bool m_productVersionSpecified;             //!< True if the product version was specified on the command line.
    bool m_componentVersionSpecified;           //!< True if the component version was specified on the command line.
    chip_family_t m_family;                     //!< Chip family that the output file is formatted for.
    elftosb::ConversionController m_controller; //!< Our conversion controller instance.
    bool m_doElftoSB;                           //!< True if generating SB file.
    // Variable used for keygen.
    bool m_doKeygen; //!< Whether to generate the key file.
    int m_keyCount;  //!< Number of keys to generate.
    // Variable used for dump
    bool m_doExtract;                          //!< True if extract mode is on.
    int32_t m_sectionIndex;                    //!< Index of section to extract
    bool m_extractBinary;                      //!< True if extraction output is binary, false for hex.
    smart_ptr<EncoreBootImageReader> m_reader; //!< Boot image reader object.
    const char* m_privateKey;                  //!< Paths to private file.
    string_vector_t m_certFilePaths;           //!< Paths to certificate files.
    string_vector_t m_rootKeyCertFilePaths;    //!< Paths to root key certificate files.
    uint32_t m_keySize;						   //!< Size of key to be generated by keygen

public:
    /*!
     * Constructor.
     *
     * Creates the singleton logger instance.
     */
    elftosbTool(int argc, char *argv[])
        : m_argc(argc)
        , m_argv(argv)
        , m_logger(0)
        , m_keyFilePaths()
        , m_positionalArgs()
        , m_isVerbose(false)
        , m_useDefaultKey(false)
        , m_commandFilePath(NULL)
        , m_outputFilePath(NULL)
        , m_hashFilePath(NULL)
        , m_searchPath(NULL)
        , m_productVersion()
        , m_componentVersion()
        , m_productVersionSpecified(false)
        , m_componentVersionSpecified(false)
        , m_family(kKinetisK3Family)
        , m_controller()
        , m_doElftoSB(false)
        , m_doKeygen(false)
        , m_keyCount(1)
        , m_doExtract(false)
        , m_sectionIndex(-1)
        , m_extractBinary(false)
        , m_reader()
        , m_certFilePaths()
        , m_rootKeyCertFilePaths()
        , m_privateKey(NULL)
		, m_keySize(128)
    {
        // create logger instance
        m_logger = new StdoutLogger();
        m_logger->setFilterLevel(Logger::INFO);
        Log::setLogger(m_logger);
    }

    /*!
     * Destructor.
     */
    ~elftosbTool() {}
    /*!
     * \brief Searches the family name table.
     *
     * \retval true The \a name was found in the table, and \a family is valid.
     * \retval false No matching family name was found. The \a family argument is not modified.
     */
    bool lookupFamilyName(const char *name, chip_family_t *family)
    {
        // Create a local read-write copy of the argument string.
        std::string familyName(name);

        // Convert the argument string to lower case for case-insensitive comparison.
        for (unsigned n = 0; n < familyName.length(); n++)
        {
            familyName[n] = tolower(familyName[n]);
        }

        // Exit the loop if we hit the NULL terminator entry.
        const FamilyNameTableEntry *entry = &kFamilyNameTable[0];
        for (; entry->name; entry++)
        {
            // Compare lowercased name with the table entry.
            if (familyName == entry->name)
            {
                *family = entry->family;
                return true;
            }
        }

        // Failed to find a matching name.
        return false;
    }

    /*!
     * Reads the command line options passed into the constructor.
     *
     * This method can return a return code to its caller, which will cause the
     * tool to exit immediately with that return code value. Normally, though, it
     * will return -1 to signal that the tool should continue to execute and
     * all options were processed successfully.
     *
     * The Options class is used to parse command line options. See
     * #k_optionsDefinition for the list of options and #k_usageText for the
     * descriptive help for each option.
     *
     * \retval -1 The options were processed successfully. Let the tool run normally.
     * \return A zero or positive result is a return code value that should be
     *		returned from the tool as it exits immediately.
     */
    int processOptions()
    {
        Options options(*m_argv, k_optionsDefinition);
        OptArgvIter iter(--m_argc, ++m_argv);

        // process command line options
        int optchar;
        const char *optarg;
        char *p;
        while ((optchar = options(iter, optarg)))
        {
            switch (optchar)
            {
                case '?':
                    printUsage(options);
                    return 0;

                case 'v':
                    printf("%s %s\n%s\n", k_toolName, k_version, k_copyright);
                    return 0;

                case 'f':
                    if (!lookupFamilyName(optarg, &m_family))
                    {
                        Log::log(Logger::ERROR, "error: unknown chip family '%s'\n", optarg);
                        printUsage(options);
                        return 0;
                    }
                    break;

                case 'c':
                    m_doElftoSB = true;
                    m_commandFilePath = optarg;
                    break;

                case 'o':
                    m_outputFilePath = optarg;
                    break;

                case 'h':
                    m_hashFilePath = optarg;
                    break;

                case 'P':
                    m_productVersion.set(optarg);
                    m_productVersionSpecified = true;
                    break;

                case 'C':
                    m_componentVersion.set(optarg);
                    m_componentVersionSpecified = true;
                    break;

                case 'k':
                    m_keyFilePaths.push_back(optarg);
                    break;

                case 'z':
                    m_useDefaultKey = true;
                    break;

                case 'D':
                    overrideVariable(optarg);
                    break;

                case 'O':
                    overrideOption(optarg);
                    break;

                case 'K':
                    m_doKeygen = true;
                    if (optarg) {
						try {
							m_keySize = stoul(optarg, nullptr, 10);
						}
						catch (exception) {
							Log::log(Logger::ERROR, "error: Invalid parameter(-K %s) expected -K [128, 256]", optarg);
							return 0;
						}
						if (m_keySize != 128 && m_keySize != 256) {
							Log::log(Logger::ERROR, "error: Invalid parameter(-K %s) expected -K [128, 256]", optarg);
							return 0;
						}
					}
                    break;

                case 'n':
                    p = NULL;
                    m_keyCount = static_cast<int>(strtoll(optarg, &p, 0));
                    if ((p == NULL) || (*p != 0))
                    {
                        Log::log(Logger::ERROR, "error: Invalid parameter(-n %s)", optarg);
                        return 0;
                    }
                    if (m_keyCount <= 0)
                    {
                        Log::log(Logger::ERROR, "error: key count must be positive.\n");
                        return 0;
                    }
                    break;

                case 'x':
                    m_doExtract = true;
                    break;

                case 'i':
                    p = NULL;
                    m_sectionIndex = strtol(optarg, &p, 0);
                    if ((p == NULL) || (*p != 0))
                    {
                        Log::log(Logger::ERROR, "error: Invalid parameter(-i %s)", optarg);
                        return 0;
                    }
                    if (m_sectionIndex < 0)
                    {
                        Log::log(Logger::ERROR, "error: section index must be non-negative.\n");
                        return 0;
                    }
                    break;

                case 'b':
                    m_extractBinary = true;
                    Log::getLogger()->setFilterLevel(Logger::WARNING);
                    break;

                case 'd':
                    Log::getLogger()->setFilterLevel(Logger::DEBUG);
                    break;

                case 'q':
                    Log::getLogger()->setFilterLevel(Logger::WARNING);
                    break;

                case 'V':
                    m_isVerbose = true;
                    break;

                case 'p':
                {
                    std::string newSearchPath(optarg);
                    PathSearcher::getGlobalSearcher().addSearchPath(newSearchPath);
                    break;
                }
                case 's':
                    m_privateKey = optarg;
                    break;
 
                case 'S':
                    m_certFilePaths.push_back(optarg);
                    break;

                case 'R':
                    m_rootKeyCertFilePaths.push_back(optarg);
                    break;

				default:
                    Log::log(Logger::ERROR, "error: unrecognized option\n\n");
                    printUsage(options);
                    return 0;
            }
        }

        // To avoid conflict, allow only one operation of elftosb, dump and keygen each time.
        if ((static_cast<int>(m_doElftoSB) + static_cast<int>(m_doExtract) + static_cast<int>(m_doKeygen)) > 1)
        {
            Log::log(Logger::ERROR, "Please do not specify \'-c\', \'-K\' and \'-x\' options at the same time.\n");
            return 0;
        }

        // handle positional args
        if (iter.index() < m_argc)
        {
            Log::SetOutputLevel leveler(Logger::DEBUG);
            Log::log("positional args:\n");
            int i;
            for (i = iter.index(); i < m_argc; ++i)
            {
                Log::log("%d: %s\n", i - iter.index(), m_argv[i]);
                m_positionalArgs.push_back(m_argv[i]);
            }
        }

        // all is well
        return -1;
    }

    /*!
     * Prints help for the tool.
     */
    void printUsage(Options &options)
    {
        options.usage(std::cout, "files...");
        printf(k_usageText, k_toolName);
    }

    /*!
     * \brief Core of the tool.
     *
     * Calls processOptions() to handle command line options before performing the
     * real work the tool does.
     */
    int run()
    {
        try
        {
            // read command line options
            int result;
            if ((result = processOptions()) != -1)
            {
                return result;
            }

            // set verbose logging
            setVerboseLogging();

            if (m_doKeygen)
            {
                if ((m_outputFilePath == NULL) && (m_positionalArgs.size() < 1))
                {
                    throw std::runtime_error(
                        "No output file path was provided.\nPlease use \'-o\' option, or provide the path at the end "
                        "of command line");
                }
                // If output file path was provided by '-o' option, write keys to that file.
                if (m_outputFilePath != NULL)
                {
                    generateKeyFile(m_outputFilePath);
                }
                // If output file path(s) was(were) provided at the end of command line, write keys to the file(s)
                // use 'if' instead of 'else if' to allow writting keys to the file(s) provided at the end of command
                // line, even '-o' is specified.
                if (m_positionalArgs.size() >= 1)
                {
                    // generate key files
                    string_vector_t::const_iterator it = m_positionalArgs.begin();
                    for (; it != m_positionalArgs.end(); ++it)
                    {
                        generateKeyFile(*it);
                    }
                }
            }
            else if (m_doExtract)
            {
                if (m_positionalArgs.size() < 1)
                {
                    throw std::runtime_error("no sb file path was provided");
                }
                if (m_positionalArgs.size() > 1)
                {
                    Log::log(Logger::WARNING,
                             "Warning: There are more than one sb file path provided. Only the first one will be "
                             "handled.\n\n");
                }

                readBootImage();
            }
            else if (m_commandFilePath != NULL)
            {
                // check argument values
                checkArguments();

                // set up the controller
                m_controller.setCommandFilePath(m_commandFilePath);

                // add external paths to controller
                string_vector_t::iterator it = m_positionalArgs.begin();
                for (; it != m_positionalArgs.end(); ++it)
                {
                    m_controller.addExternalFilePath(*it);
                }

                // run conversion
                convert();
            }
            else
            {
                throw std::runtime_error(
                    "Unknown Operation.\nPlease use one of \'-c\', \'-K\' and \'-x\' options to specify your "
                    "operation.");
            }
        }
        catch (std::exception &e)
        {
            Log::log(Logger::ERROR, "error: %s\n", e.what());
            return 1;
        }
        catch (...)
        {
            Log::log(Logger::ERROR, "error: unexpected exception\n");
            return 1;
        }

        return 0;
    }

    /*!
     * \brief Validate arguments that can be checked.
     * \exception std::runtime_error Thrown if an argument value fails to pass validation.
     */
    void checkArguments()
    {
        if (m_commandFilePath == NULL)
        {
            throw std::runtime_error("no command file was specified");
        }
        if (m_outputFilePath == NULL)
        {
            throw std::runtime_error("no output file was specified");
        }
    }
    /*!
     * \brief Turns on verbose logging.
     */
    void setVerboseLogging()
    {
        if (m_isVerbose)
        {
            // verbose only affects the INFO and DEBUG filter levels
            // if the user has selected quiet mode, it overrides verbose
            switch (Log::getLogger()->getFilterLevel())
            {
                case Logger::INFO:
                    Log::getLogger()->setFilterLevel(Logger::INFO2);
                    break;
                case Logger::DEBUG:
                    Log::getLogger()->setFilterLevel(Logger::DEBUG2);
                    break;
                default:;
            }
        }
    }

    /*!
     * \brief Returns the integer value for a string.
     *
     * Metric multiplier prefixes are supported.
     */
    uint32_t parseIntValue(const char *value)
    {
        // Accept 'true'/'yes' and 'false'/'no' as integer values.
        if ((strcmp(value, "true") == 0) || (strcmp(value, "yes") == 0))
        {
            return 1;
        }
        else if ((strcmp(value, "false") == 0) || (strcmp(value, "no") == 0))
        {
            return 0;
        }

        uint32_t intValue = strtoul(value, NULL, 0);
        unsigned multiplier;
        switch (value[strlen(value) - 1])
        {
            case 'G':
                multiplier = 1024 * 1024 * 1024;
                break;
            case 'M':
                multiplier = 1024 * 1024;
                break;
            case 'K':
                multiplier = 1024;
                break;
            default:
                multiplier = 1;
        }
        intValue *= multiplier;
        return intValue;
    }

    /*!
     * \brief Parses the -D option to override a constant value.
     */
    void overrideVariable(const char *optarg)
    {
        // split optarg into two strings
        std::string constName(optarg);
        unsigned i;
        for (i = 0; i < strlen(optarg); ++i)
        {
            if (optarg[i] == '=')
            {
                constName.resize(i++);
                break;
            }
        }

        uint32_t constValue = parseIntValue(&optarg[i]);

        elftosb::EvalContext &context = m_controller.getEvalContext();
        context.setVariable(constName, constValue);
        context.lockVariable(constName);
    }

    /*!
     * \brief
     */
    void overrideOption(const char *optarg)
    {
        // split optarg into two strings
        std::string optionName(optarg);
        unsigned i;
        for (i = 0; i < strlen(optarg); ++i)
        {
            if (optarg[i] == '=')
            {
                optionName.resize(i++);
                break;
            }
        }

        // handle quotes for option value
        const char *valuePtr = &optarg[i];
        bool isString = false;
        int len;
        if (valuePtr[0] == '"')
        {
            // remember that the value is a string and get rid of the opening quote
            isString = true;
            valuePtr++;

            // remove trailing quote if present
            len = strlen(valuePtr);
            if (valuePtr[len] == '"')
            {
                len--;
            }
        }

        elftosb::Value *value;
        if (isString)
        {
            std::string stringValue(valuePtr);
            stringValue.resize(len); // remove trailing quote
            value = new elftosb::StringValue(stringValue);
        }
        else
        {
            value = new elftosb::IntegerValue(parseIntValue(valuePtr));
        }

        // Set and lock the option in the controller
        m_controller.setOption(optionName, value);
        m_controller.lockOption(optionName);
    }

    /*!
     * \brief Do the conversion.
     * \exception std::runtime_error This exception is thrown if the conversion controller does
     *		not produce a boot image, or if the output file cannot be opened. Other errors
     *		internal to the conversion controller may also produce this exception.
     */
    void convert()
    {
        // Set boot image type
        int typeBootImage = BootImageGenerator::SB;
        // create a generator for the chosen chip family
        smart_ptr<elftosb::BootImageGenerator> generator;
        switch (m_family)
        {
            case k37xxFamily:
                generator = new elftosb::EncoreBootImageGenerator;
                elftosb::g_enableHABSupport = false;
                break;

            case kMX28Family:
                generator = new elftosb::EncoreBootImageGenerator;
                elftosb::g_enableHABSupport = true;
                break;

            case kKinetisFamily:
                generator = new elftosb::EncoreBootImageGenerator;
                elftosb::g_enableHABSupport = false;
                break;
            case kKinetisK3Family:
                generator = new elftosb::SB2ImageGenerator;
                elftosb::g_enableHABSupport = false;
                typeBootImage = BootImageGenerator::SB2;
                break;
			case kiMXFamily:
				generator = new elftosb::iMXImageGenerator;
				elftosb::g_enableHABSupport = false;
				typeBootImage = BootImageGenerator::iMX;
				break;

        }

        // process input and get a boot image
        m_controller.run();
        smart_ptr<elftosb::BootImage> image = m_controller.generateOutput(generator);
        if (!image)
        {
            throw std::runtime_error("failed to produce output!");
        }

        // set version numbers if they were provided on the command line
        if (m_productVersionSpecified)
        {
            image->setProductVersion(m_productVersion);
        }
        if (m_componentVersionSpecified)
        {
            image->setComponentVersion(m_componentVersion);
        }

        // special handling for each family
        switch (m_family)
        {
            case k37xxFamily:
            case kMX28Family:
            case kKinetisFamily:
            {
                // add OTP keys
                elftosb::EncoreBootImage *encoreImage = dynamic_cast<elftosb::EncoreBootImage *>(image.get());
                if (encoreImage)
                {
                    // add keys
                    addCryptoKeys(encoreImage);

                    // print debug image
                    encoreImage->debugPrint();
                }
                break;
            }
            case kKinetisK3Family:
            {
                // add OTP keys
                elftosb::SB2Image *SB2Image = dynamic_cast<elftosb::SB2Image *>(image.get());
                if (SB2Image)
                {
                    // If image is encrypted but no key is provided nor zero key option then default to zero key
                    if ((m_keyFilePaths.size() == 0) && (m_useDefaultKey == false))
                    {
                        m_useDefaultKey = true;
                    }

                    if (SB2Image->isSigned() && (m_certFilePaths.size() == 0 || m_privateKey == NULL || m_rootKeyCertFilePaths.size() == 0))
                    {
                        throw std::runtime_error(format_string("Signed image require private key with -s option, one or more certificate(s) using -S option and one or more root key certificates using -R option"));
                    }
                    // add keys and certs
                    addSB2CryptoKeys(SB2Image);
                    
                    // add certs
                    // print debug image
                    SB2Image->debugPrint();
                }
                break;
            }
			case kiMXFamily:
			{
				// add OTP keys
				elftosb::iMXImage *iMXImage = dynamic_cast<elftosb::iMXImage *>(image.get());
				if (iMXImage)
				{

					iMXImage->setOutputFilePath(m_outputFilePath);
					iMXImage->setSourceFile(m_controller.getSourceFile(SOURCE_FILE_TAG_NAME));

					// If image is encrypted but no key is provided nor zero key option then default to zero key
/*
					if ((m_keyFilePaths.size() == 0) && (m_useDefaultKey == false))
					{
						m_useDefaultKey = true;
					}

					if (iMXImage->isSigned() && (m_certFilePaths.size() == 0 || m_privateKey == NULL || m_rootKeyCertFilePaths.size() == 0))
					{
						throw std::runtime_error(format_string("Signed image require private key with -s option, one or more certificate(s) using -S option and one or more root key certificates using -R option"));
					}
*/
					// add keys and certs
					// FM:todo 
					// addSB2CryptoKeys(SB2Image);

					// add certs
					// print debug image
					iMXImage->debugPrint();
				}
				break;
			}
		}

        // write output
        std::ofstream outputStream(m_outputFilePath, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
        if (outputStream.is_open())
        {
            image->writeToStream(outputStream);
        }
        else
        {
            throw std::runtime_error(format_string("could not open output file %s", m_outputFilePath));
        }
    }

    /*!
     * \brief
     */
    void addCryptoKeys(elftosb::EncoreBootImage *encoreImage)
    {
        string_vector_t::iterator it = m_keyFilePaths.begin();
        for (; it != m_keyFilePaths.end(); ++it)
        {
            std::string &keyPath = *it;

            std::string actualPath;
            bool found = PathSearcher::getGlobalSearcher().search(keyPath, PathSearcher::kFindFile, true, actualPath);
            if (!found)
            {
                throw std::runtime_error(format_string("unable to find key file %s\n", keyPath.c_str()));
            }

            std::ifstream keyStream(actualPath.c_str(), std::ios_base::in);
            if (!keyStream.is_open())
            {
                throw std::runtime_error(format_string("unable to read key file %s\n", keyPath.c_str()));
            }
            keyStream.seekg(0);

            try
            {
                // read as many keys as possible from the stream
                while (true)
                {
                    AESKey<128> key(keyStream);
                    encoreImage->addKey(key);

                    // dump key bytes
                    dumpKey(key);
                }
            }
            catch (...)
            {
                // ignore the exception -- there are just no more keys in the stream
            }
        }

        // add the default key of all zero bytes if requested
        if (m_useDefaultKey)
        {
            AESKey<128> defaultKey;
            encoreImage->addKey(defaultKey);
        }
    }

    /*!
    * \brief
    */
    void addSB2CryptoKeys(elftosb::SB2Image *sb2Image)
    {
        // add kek
        string_vector_t::iterator it = m_keyFilePaths.begin();
        for (; it != m_keyFilePaths.end(); ++it)
        {
            std::string &keyPath = *it;

            std::string actualPath;
            bool found = PathSearcher::getGlobalSearcher().search(keyPath, PathSearcher::kFindFile, true, actualPath);
            if (!found)
            {
                throw std::runtime_error(format_string("unable to find key file %s\n", keyPath.c_str()));
            }

            std::ifstream keyStream(actualPath.c_str(), std::ios_base::in);
            if (!keyStream.is_open())
            {
                throw std::runtime_error(format_string("unable to read key file %s\n", keyPath.c_str()));
            }
            keyStream.seekg(0);

            try
            {
                // Limit to one key per -k argument
                AESKey<256> kek(keyStream);
                sb2Image->addKey(kek);

                // dump key bytes
                dumpKey(kek);
            }
            catch (std::exception &e)
            {
                throw std::runtime_error(format_string("Error in the key file, %s: %s\n", keyPath.c_str(), e.what()));
            }
        }

        // add the default key of all zero bytes if requested
        if (m_useDefaultKey)
        {
            AESKey<256> defaultKey;
            defaultKey.allOnes();
            sb2Image->addKey(defaultKey);
            dumpKey(defaultKey);
        }

        AESKey<256> dek, macKey;
        dek.randomize();
        macKey.randomize();
        sb2Image->setDek(dek);
        sb2Image->setMacKey(macKey);
        dumpKey(dek);
        dumpKey(macKey);

        
        // add private key
        sb2Image->setPrivKeyPath(m_privateKey);
        
        // add certificate files
        sb2Image->addCerts(m_certFilePaths);

        // add root key certificate files
        sb2Image->addRootKeyCerts(m_rootKeyCertFilePaths);

        // add output hash file
        sb2Image->addHashFileName(m_hashFilePath);
    }

    /*!
    * \brief Opens the file at \a path and writes a random key file.
    *
    * Each key file will have #m_keyCount number of keys written into it,
    * each on a line by itself.
    */
    void generateKeyFile(const std::string &path)
    {
        std::ofstream outputStream(path.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
        if (outputStream.is_open())
        {
            if (m_keySize == 128) {
				for (int i = 0; i < m_keyCount; ++i) {
					AESKey<128> key;
					key.randomize();
					key.writeToStream(outputStream);
					dumpKey(key);
					// put a newline after the key
					outputStream.write("\n", 1);
				}
			}
			else if (m_keySize == 256) {
				for (int i = 0; i < m_keyCount; ++i) {
					AESKey<256> key;
					key.randomize();
					key.writeToStream(outputStream);
					dumpKey(key);
					// put a newline after the key
					outputStream.write("\n", 1);
				}
			}
			else {
				stringstream ss("Wrong value of -K (");
				ss << m_keySize << "), supported only -K [128, 256].";
				throw std::runtime_error(ss.str());
			}

            Log::log(Logger::INFO, "wrote key file %s\n", path.c_str());
        }
        else {
            throw std::runtime_error("could not open output file");
        }
    }

    /*!
     * \brief Write the value of each byte of the \a key to the log.
     */
    void dumpKey(const AESKey<128> &key)
    {
        // dump key bytes
        Logger::log_level_t logLevel;
        // If generating key, then print the keys to INFO level. so user can use -V parameter to see them.
        if (m_doKeygen)
            logLevel = Logger::INFO2;
        // Otherwise, printf the keys to debug level, only -d parameter can print the keys to stdout.
        else
            logLevel = Logger::DEBUG;

        Log::log(logLevel, "key bytes: ");
        AESKey<128>::key_t the_key;
        key.getKey(&the_key);
        int q;
        for (q = 0; q < 16; q++)
        {
            Log::log(logLevel, "%02x ", the_key[q]);
        }
        Log::log(logLevel, "\n");
    }

    /*!
    * \brief Write the value of each byte of the \a key to the log.
    */
    void dumpKey(const AESKey<256> &key)
    {
        // dump key bytes
        Logger::log_level_t logLevel;
        if (m_doKeygen)
            logLevel = Logger::INFO2;
        // Otherwise, printf the keys to debug level, only -d parameter can print the keys to stdout.
        else
            logLevel = Logger::DEBUG;

        Log::log(logLevel, "key bytes: ");
        AESKey<256>::key_t the_key;
        key.getKey(&the_key);
        int q;
        for (q = 0; q < 32; q++)
        {
            Log::log(logLevel, "%02x ", the_key[q]);
        }
        Log::log(logLevel, "\n");
    }


    /*!
    * \brief Opens and reads the boot image identified on the command line.
    * \pre At least one position argument must be present.
    */
    void readBootImage()
    {
        Log::SetOutputLevel infoLevel(Logger::INFO);

        // open the sb file stream
        std::ifstream sbStream(m_positionalArgs[0].c_str(), std::ios_base::binary | std::ios_base::in);
        if (!sbStream.is_open())
        {
            throw std::runtime_error("failed to open input file");
        }

        // create the boot image reader
        m_reader = new EncoreBootImageReader(sbStream);

        // read image header
        m_reader->readImageHeader();
        const EncoreBootImage::boot_image_header_t &header = m_reader->getHeader();
        if (header.m_majorVersion > 1)
        {
            throw std::runtime_error(format_string("boot image format version is too new (format version %d.%d)\n",
                                                   header.m_majorVersion, header.m_minorVersion));
        }
        Log::log("---- Boot image header ----\n");
        dumpImageHeader(header);

        // compute SHA-1 over image header and test against the digest stored in the header
        sha1_digest_t computedDigest;
        m_reader->computeHeaderDigest(computedDigest);
        if (compareDigests(computedDigest, m_reader->getHeader().m_digest))
        {
            Log::log("Header digest is correct.\n");
        }
        else
        {
            Log::log(Logger::WARNING, "warning: stored SHA-1 header digest does not match the actual header digest\n");
            Log::log(Logger::WARNING, "\n---- Actual SHA-1 digest of image header ----\n");
            logHexArray(Logger::WARNING, (uint8_t *)&computedDigest, sizeof(computedDigest));
        }

        // read the section table
        m_reader->readSectionTable();
        const EncoreBootImageReader::section_array_t &sectionTable = m_reader->getSections();
        EncoreBootImageReader::section_array_t::const_iterator it = sectionTable.begin();
        Log::log("\n---- Section table ----\n");
        unsigned n = 0;
        for (; it != sectionTable.end(); ++it, ++n)
        {
            const EncoreBootImage::section_header_t &sectionHeader = *it;
            Log::log("Section %d:\n", n);
            dumpSectionHeader(sectionHeader);
        }

        // read the key dictionary
        // XXX need to support multiple keys, not just the first!
        if (m_reader->isEncrypted())
        {
            Log::log("\n---- Key dictionary ----\n");
            if (m_keyFilePaths.size() > 0 || m_useDefaultKey)
            {
                if (m_keyFilePaths.size() > 0)
                {
                    std::string &keyPath = m_keyFilePaths[0];
                    std::ifstream keyStream(keyPath.c_str(), std::ios_base::binary | std::ios_base::in);
                    if (!keyStream.is_open())
                    {
                        Log::log(Logger::WARNING, "warning: unable to read key %s\n", keyPath.c_str());
                    }
                    AESKey<128> kek(keyStream);

                    // search for this key in the key dictionary
                    if (!m_reader->readKeyDictionary(kek))
                    {
                        throw std::runtime_error("the provided key is not valid for this encrypted boot image");
                    }

                    Log::log("\nKey %s was found in key dictionary.\n", keyPath.c_str());
                }
                else
                {
                    // default key of zero, overriden if -k was used
                    AESKey<128> defaultKek;

                    // search for this key in the key dictionary
                    if (!m_reader->readKeyDictionary(defaultKek))
                    {
                        throw std::runtime_error("the default key is not valid for this encrypted boot image");
                    }

                    Log::log("\nDefault key was found in key dictionary.\n");
                }

                // print out the DEK
                AESKey<128> dek = m_reader->getKey();
                std::stringstream dekStringStream(std::ios_base::in | std::ios_base::out);
                dek.writeToStream(dekStringStream);
                std::string dekString = dekStringStream.str();
                // 				Log::log("\nData encryption key: %s\n", dekString.c_str());
                Log::log("\nData encryption key:\n");
                logHexArray(Logger::INFO, (const uint8_t *)&dek.getKey(), sizeof(AESKey<128>::key_t));
            }
            else
            {
                throw std::runtime_error("the image is encrypted but no key was provided");
            }
        }

        // read the SHA-1 digest over the entire image. this is done after
        // reading the key dictionary because the digest is encrypted in
        // encrypted boot images.
        m_reader->readImageDigest();
        const sha1_digest_t &embeddedDigest = m_reader->getDigest();
        Log::log("\n---- SHA-1 digest of entire image ----\n");
        logHexArray(Logger::INFO, (const uint8_t *)&embeddedDigest, sizeof(embeddedDigest));

        // compute the digest over the entire image and compare
        m_reader->computeImageDigest(computedDigest);
        if (compareDigests(computedDigest, embeddedDigest))
        {
            Log::log("Image digest is correct.\n");
        }
        else
        {
            Log::log(Logger::WARNING, "warning: stored SHA-1 digest does not match the actual digest\n");
            Log::log(Logger::WARNING, "\n---- Actual SHA-1 digest of entire image ----\n");
            logHexArray(Logger::WARNING, (uint8_t *)&computedDigest, sizeof(computedDigest));
        }

        // read the boot tags
        m_reader->readBootTags();
        Log::log("\n---- Boot tags ----\n");
        unsigned block = header.m_firstBootTagBlock;
        const EncoreBootImageReader::boot_tag_array_t &tags = m_reader->getBootTags();
        EncoreBootImageReader::boot_tag_array_t::const_iterator tagIt = tags.begin();
        for (n = 0; tagIt != tags.end(); ++tagIt, ++n)
        {
            const EncoreBootImage::boot_command_t &command = *tagIt;
            Log::log("%04u: @ block %06u | id=0x%08x | length=%06u | flags=0x%08x\n", n, block, command.m_address,
                     command.m_count, command.m_flags);

            if (command.m_flags & EncoreBootImage::ROM_SECTION_BOOTABLE)
            {
                Log::log("        0x1 = ROM_SECTION_BOOTABLE\n");
            }

            if (command.m_flags & EncoreBootImage::ROM_SECTION_CLEARTEXT)
            {
                Log::log("        0x2 = ROM_SECTION_CLEARTEXT\n");
            }

            block += command.m_count + 1;
        }

        // now read all of the sections
        Log::log(Logger::INFO2, "\n---- Sections ----\n");

        if (m_sectionIndex >= header.m_sectionCount)
        {
            Log::log(Logger::WARNING, "\nWarning: Invalid section index(%d), Only %d section%s in the sb file.",
                     m_sectionIndex, header.m_sectionCount, header.m_sectionCount > 1 ? "s" : "");
        }
        else
        {
            for (n = 0; n < header.m_sectionCount; ++n)
            {
                EncoreBootImage::Section *section = m_reader->readSection(n);
                section->debugPrint();

                // Check if this is the section the user wants to extract.
                if (m_doExtract && n == m_sectionIndex)
                {
                    extractSection(section);
                }
            }
        }
    }

    //! \brief Dumps the contents of a section to stdout.
    //!
    //! If #m_extractBinary is true then the contents are written as
    //! raw binary to stdout. Otherwise the data is formatted using
    //! logHexArray().
    void extractSection(EncoreBootImage::Section *section)
    {
        // Allocate buffer to hold section data.
        unsigned blockCount = section->getBlockCount();
        unsigned dataLength = sizeOfCipherBlocks(blockCount);
        smart_array_ptr<uint8_t> buffer = new uint8_t[dataLength];
        cipher_block_t *data = reinterpret_cast<cipher_block_t *>(buffer.get());

        // Read section data into the buffer one block at a time.
        unsigned offset;
        for (offset = 0; offset < blockCount;)
        {
            unsigned blocksRead = section->getBlocks(offset, 1, data);
            offset += blocksRead;
            data += blocksRead;
        }

        // If output file is specified , then dump the extracted data to that file.
        if (m_outputFilePath != NULL)
        {
            FILE *outputFile = fopen(m_outputFilePath, "wb");
            if (fwrite(buffer.get(), 1, dataLength, outputFile) != dataLength)
            {
                int err = ferror(outputFile);
                fclose(outputFile);
                throw std::runtime_error(
                    format_string("failed to write data to the file \"%s\" (%d)", m_outputFilePath, err));
            }
            fclose(outputFile);
            Log::log(Logger::INFO, "\n Section %d contents have been written to the file \"%s\".\n", m_sectionIndex,
                     m_outputFilePath);
        }
        // If '-b' is specified, then dump binary contents to stdout.
        else if (m_extractBinary)
        {
#if WIN32
            _setmode(_fileno(stdout), _O_BINARY);
#endif
            if (fwrite(buffer.get(), 1, dataLength, stdout) != dataLength)
            {
                throw std::runtime_error(format_string("failed to write data to stdout (%d)", ferror(stdout)));
            }
        }
        else
        {
            // Print header.
            Log::log(Logger::INFO, "\nSection %d contents:\n", m_sectionIndex);
            // Use the warning log level so the data will be visible even in quiet mode.
            logHexArray(Logger::WARNING, buffer, dataLength);
        }
    }

    //! \brief Compares two SHA-1 digests and returns whether they are equal.
    //! \retval true The two digests are equal.
    //! \retval false The \a a and \a b digests are different from each other.
    bool compareDigests(const sha1_digest_t &a, const sha1_digest_t &b)
    {
        return memcmp(a, b, sizeof(sha1_digest_t)) == 0;
    }

    void dumpImageHeader(const EncoreBootImage::boot_image_header_t &header)
    {
        version_t vers;

// The header timestamp is the number of microseconds since this epoch.
#if WIN32
        struct tm epoch = { 0, 0, 0, 1, 0, 100, 0, 0 }; // 00:00 1-1-2000
#else
        struct tm epoch = { 0, 0, 0, 1, 0, 100, 0, 0, 1, 0, NULL }; // 00:00 1-1-2000
#endif
        time_t epochTime = mktime(&epoch);

        uint64_t timestampSeconds = header.m_timestamp / 1000000; // Convert from microseconds to seconds.
        time_t timestampTime = epochTime + timestampSeconds;
        char *timestampString = asctime(localtime(&timestampTime));

        Log::SetOutputLevel infoLevel(Logger::INFO);
        Log::log("Signature 1:           %c%c%c%c\n", header.m_signature[0], header.m_signature[1],
                 header.m_signature[2], header.m_signature[3]);
        Log::log("Signature 2:           %c%c%c%c\n", header.m_signature2[0], header.m_signature2[1],
                 header.m_signature2[2], header.m_signature2[3]);
        Log::log("Format version:        %d.%d\n", header.m_majorVersion, header.m_minorVersion);
        Log::log("Flags:                 0x%04x\n", header.m_flags);
        Log::log("Image blocks:          %u\n", header.m_imageBlocks);
        Log::log("First boot tag block:  %u\n", header.m_firstBootTagBlock);
        Log::log("First boot section ID: 0x%08x\n", header.m_firstBootableSectionID);
        Log::log("Key count:             %u\n", header.m_keyCount);
        Log::log("Key dictionary block:  %u\n", header.m_keyDictionaryBlock);
        Log::log("Header blocks:         %u\n", header.m_headerBlocks);
        Log::log("Section count:         %u\n", header.m_sectionCount);
        Log::log("Section header size:   %u\n", header.m_sectionHeaderSize);
        Log::log("Timestamp:             %s (%llu)\n", timestampString, header.m_timestamp);
        vers = header.m_productVersion;
        vers.fixByteOrder();
        Log::log("Product version:       %x.%x.%x\n", vers.m_major, vers.m_minor, vers.m_revision);
        vers = header.m_componentVersion;
        vers.fixByteOrder();
        Log::log("Component version:     %x.%x.%x\n", vers.m_major, vers.m_minor, vers.m_revision);
        if (header.m_majorVersion == 1 && header.m_minorVersion >= 1)
        {
            Log::log("Drive tag:             0x%04x\n", header.m_driveTag);
        }
        Log::log("SHA-1 digest of header:\n");
        logHexArray(Logger::INFO, (uint8_t *)&header.m_digest, sizeof(header.m_digest));
    }

    void dumpSectionHeader(const EncoreBootImage::section_header_t &header)
    {
        Log::SetOutputLevel infoLevel(Logger::INFO);
        Log::log("    Identifier: 0x%x\n", header.m_tag);
        Log::log("    Offset:     %d block%s (%d bytes)\n", header.m_offset, header.m_offset != 1 ? "s" : "",
                 sizeOfCipherBlocks(header.m_offset));
        Log::log("    Length:     %d block%s (%d bytes)\n", header.m_length, header.m_length != 1 ? "s" : "",
                 sizeOfCipherBlocks(header.m_length));
        Log::log("    Flags:      0x%08x\n", header.m_flags);

        if (header.m_flags & EncoreBootImage::ROM_SECTION_BOOTABLE)
        {
            Log::log("                0x1 = ROM_SECTION_BOOTABLE\n");
        }

        if (header.m_flags & EncoreBootImage::ROM_SECTION_CLEARTEXT)
        {
            Log::log("                0x2 = ROM_SECTION_CLEARTEXT\n");
        }
    }

    /*!
    * \brief Log an array of bytes as hex.
    */
    void logHexArray(Logger::log_level_t level, const uint8_t *bytes, unsigned count)
    {
        Log::SetOutputLevel leveler(level);

        unsigned i;
        for (i = 0; i < count; ++i, ++bytes)
        {
            if ((i % 16 == 0) && (i < count - 1))
            {
                if (i != 0)
                {
                    Log::log("\n");
                }
                Log::log("    0x%08x: ", i);
            }
            Log::log("%02x ", *bytes & 0xff);
        }

        Log::log("\n");
    }
};

const elftosbTool::FamilyNameTableEntry elftosbTool::kFamilyNameTable[] = { { "37xx", k37xxFamily },
                                                                            { "377x", k37xxFamily },
                                                                            { "378x", k37xxFamily },
                                                                            { "mx23", k37xxFamily },
                                                                            { "imx23", k37xxFamily },
                                                                            { "i.mx23", k37xxFamily },
                                                                            { "mx28", kMX28Family },
                                                                            { "imx28", kMX28Family },
                                                                            { "i.mx28", kMX28Family },
                                                                            { "kinetis", kKinetisFamily },
                                                                            { "kinetisk3", kKinetisK3Family },
																			{ "imx", kiMXFamily },
                                                                            // Null terminator entry.
                                                                            { NULL, k37xxFamily } };

/*!
 * Main application entry point. Creates an sbtool instance and lets it take over.
 */
int main(int argc, char *argv[], char *envp[])
{
    try
    {
        return elftosbTool(argc, argv).run();
    }
    catch (...)
    {
        Log::log(Logger::ERROR, "error: unexpected exception\n");
        return 1;
    }

    return 0;
}
