/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_SB2Image_h_)
#define _SB2Image_h_

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include "BootImage.h"
#include "rijndael.h"
#include "smart_ptr.h"
#include "AESKey.h"
#include "StExecutableImage.h"
#include "mbedtls/sha256.h"

namespace elftosb
{
//! An AES-256 cipher block is bytes.
typedef uint8_t cipher_block_t[16];

//! A SHA-256 digest is 256 bits, or 32 bytes.
typedef uint8_t sha256_digest_t[32];

//! Unique identifier type for a section.
typedef uint32_t section_id_t;

//! An array of strings.
typedef std::vector<std::string> string_vector_t;

/*!
 * \brief Class to manage SB2 image files.
 *
 * Initially this class will only support generation of boot images, but
 * its design will facilitate the addition of the ability to read an
 * image and examine its contents.
 *
 * A boot image is composed of the following regions:
 * - Header
 * - HMAC on header
 * - Key blob
 * - Section data (incl sign section)
 * - Authentication
 *
 * Multiple sections are within a boot image are fully supported. Two general types
 * of sections are supported with this class. Bootable sections, represented by the
 * SB2Image::BootSection class, contain a sequence of commands to be
 * interpreted by the boot ROM. Data sections are represented by the
 * SB2Image::DataSection class and can contain any arbitrary data.
 *
 * An image can either be encrypted or unencrypted. The image uses a session key,
 * or DEK (data encryption key), and the key dictionary to support any number of keys
 * using a single image. The header and section table are always unencrypted even
 * in encrypted images. This allows host utilities to access the individual
 * sections without needing to have access to an encryption key.
 *
 * To construct a boot image, first create an instance of SB2Image. Then
 * create instances of the SB2Image::BootSection or SB2Image::DataSection
 * for each of the sections in the image. For bootable sections, create and add
 * the desired boot command objects. These are all subclasses of
 * SB2Image::BootCommand.
 *
 * If the boot image is to be encrypted, you need to add keys, which are instances
 * of the AES128Key class. If no keys are added, the entire boot image will be unencrypted.
 *
 * When the image is fully constructed, it can be written to any std::ostream with
 * a call to writeToStream(). The same image can be written to streams any
 * number of times.
 */
class SB2Image : public BootImage
{
public:
    
    //! \brief shifters for the flag constants
    enum
    {
        ROM_DISPLAY_PROGRESS_SHIFT = 0,
        ROM_VERBOSE_PROGRESS_SHIFT = 1,
        ROM_ENCRYPTED_SHIFT = 2,
        ROM_SIGNED_SHIFT = 3,
        CIPHER_MODE_SHIFT = 4
    };

    //! \brief Flag constants for the m_flags field of #elftosb::SB2BootImage::sb2_header_t.
    enum
    {
        ROM_DISPLAY_PROGRESS = (1 << ROM_DISPLAY_PROGRESS_SHIFT), //!< Print progress reports.
        ROM_VERBOSE_PROGRESS = (1 << ROM_VERBOSE_PROGRESS_SHIFT), //!< Progress reports are verbose.
        ROM_ENCRYPTED = (1 << ROM_ENCRYPTED_SHIFT), //!< The SB file is encrypted if set. keyBlobBlock and keyBlobCount must be set appropriately
        ROM_SIGNED = (1 << ROM_SIGNED_SHIFT),    //!< The SB file is signed. offsetToCertificateBlockHeaderInBytes must be set appropriately
        ROM_CIPHER_MODE = (3 << CIPHER_MODE_SHIFT)       //!< Block cipher mode used to encrypt section data.
    };
    //! \brief Cipher mode flag values
    enum
    {
        CIPHER_MODE_AES_CTR = 0, //!< 2/b00 - AES-CTR
        CIPHER_MODE_RESERVED     //!< 2'b01 - 2b'11 are reserved
    };

    enum
    {
        ROM_IMAGE_HEADER_SIGNATURE =
            'STMP', //!< Signature in #elftosb::SB2Image::sb2_header_t::m_signature.
        ROM_IMAGE_HEADER_SIGNATURE2 =
            'sgtl',                       //!< Value for #elftosb::SB2Image::sb2_header_t::m_signature2;
        ROM_BOOT_IMAGE_MAJOR_VERSION = 2, //!< Current boot image major version.
        ROM_BOOT_IMAGE_MINOR_VERSION = 0  //!< Current boot image minor version.
    };

    enum
    {
        //! Minimum alignment for a section is 16 bytes.
        BOOT_IMAGE_MINIMUM_SECTION_ALIGNMENT = sizeof(cipher_block_t)
    };

    enum
    {
        //! Root key hash table size
        MAX_ROOT_KEY_HASH_TABLE_SIZE = 4
    };
// All of these structures are packed to byte alignment in order to match
// the structure on disk.
#pragma pack(1)

    //! \brief Header for the entire boot image.
    //!
    //! Fields of this header are arranged so that those used by the bootloader ROM all come
    //! first. They are also set up so that all fields are not split across cipher block
    //! boundaries. The fields not used by the bootloader are not subject to this
    //! restraint.
    //!
    //! Image header size is always a round number of cipher blocks. The same also applies to
    //! the boot image itself. The padding, held in #elftosb::SB2Image::sb2_header_t::m_padding0
    //! and #elftosb::SB2Image::sb2_header_t::m_padding1 is filled with random bytes.
    //!
    //! The DEK dictionary, section table, and each section data region must all start on
    //! cipher block boundaries.
    //!
    //! This header is not encrypted in the image file.
    //!
    //! The m_digest field contains a SHA-1 digest of the fields of the header that follow it.
    //! It is the first field in the header so it doesn't change position or split the header
    //! in two if fields are added to the header.
    struct sb2_header_t
    {
        uint32_t nonce[4];            //!< Nonce for AES-CTR
        uint32_t reserved;            //!< Reserved, un-used
        uint8_t m_signature[4];       //!< 'STMP', see #ROM_IMAGE_HEADER_SIGNATURE.
        uint8_t m_majorVersion;       //!< Major version for the image format, see #ROM_BOOT_IMAGE_MAJOR_VERSION.
        uint8_t m_minorVersion;       //!< Minor version of the boot image format, see #ROM_BOOT_IMAGE_MINOR_VERSION.
        uint16_t m_flags;             //!< Flags or options associated with the entire image.
        uint32_t m_imageBlocks;       //!< Size of entire image in blocks.
        uint32_t m_firstBootTagBlock; //!< Offset from start of file to the first boot tag, in blocks.
        section_id_t m_firstBootableSectionID; //!< ID of section to start booting from.
        uint32_t m_offsetToCertificateBlockInBytes;     //! Offset in bytes to the certificate block header for a signed SB file.
        uint16_t m_headerBlocks;               //!< Size of this header, including this size word, in blocks.
        uint16_t m_keyBlobBlock;      //!< Block number where the key blob starts
        uint16_t m_keyBlobBlockCount; //!< Number of cipher blocks occupied by the key blob. 
        uint16_t m_maxSectionMacCount; //!< Maximum number of HMAC table entries used in all sections of the SB file.
        uint8_t m_signature2[4];      //!< Always set to 'sgtl'
        uint64_t m_timestamp;         //!< Timestamp when image was generated in microseconds since 1-1-2000.
        version_t m_productVersion;   //!< User controlled product version.
        version_t m_componentVersion; //!< User controlled component version.
        uint32_t m_buildNumber;          //!< User controlled build number.
        uint8_t m_padding1[4];        //!< Padding to round up to next cipher block.
    };

   //! \brief Information about each section
    struct section_header_t
    {
        uint32_t m_tag;    //!< Unique identifier for this section. High bit must be zero.
        uint32_t m_offset; //!< Offset to section data from start of image in blocks.
        uint32_t m_length; //!< Size of section data in blocks.
        uint32_t m_flags;  //!< Section flags.
    };

    //! \brief The certificate block header (or just certificate header) is a structure containing 
    //! information required to properly verify a signed image. 
    //! It is pointed to by the header offset field: m_offsetToCertificateBlockInBytes
    //! The certificate header is allowed to reside anywhere within the signed image.
    struct certificate_block_header_t {
        uint8_t  signature[4];                  //!< Always set to 'cert'
        uint16_t headerMajorVersion;            //!< Set to 1
        uint16_t headerMinorVersion;            //!< Set to 0
        uint32_t headerLengthInBytes;           //!< Starting from the signature and not including the certificate table.
        uint32_t flags;                         //!< Reserved for future use. 
        uint32_t buildNumber;                   //!< Build number of the user code. Allows user to prevent reverting to old versions
        uint32_t totalImageLengthInBytes;       //!< Length in bytes of the signed data
        uint32_t certificateCount;              //!< Must be greater than 0
        uint32_t certificateTableLengthInBytes; //!< Total length in bytes of the certificate table
    };

    //! \brief Immediately following the certificate block header in the image is the certificate table. 
    //! In other words, the certificate table is located at offset (offsetToCertificateBlockHeaderInBytes 
    //!     + headerLengthInBytes)
    struct cert_entry_t {
        uint32_t x509CertificateLengthInBytes; //!< Must be set to the length of the certificate data in bytes
        //uint32_t alignedCertLength;            //!< Aligned length to word boundary
        uint8_t *x509Certificate;            //!< Pointer to certificate data
    };

    //! \brief Certificates table holding an array of cert_entry
    struct cert_table_t {
        cert_entry_t *entries; //!< Certificate entries
    };

    //! \brief sha256 hash of the root key 
    struct rkh_entry {
        sha256_digest_t rkh;
    };

    //! \brief Table holding sha256 hash of up to 4 root keys. 
    //! This table follows the certificate table in the SB2.0 image
    struct rkh_table {
        rkh_entry entries[MAX_ROOT_KEY_HASH_TABLE_SIZE];
    };

    //! \brief Structure for a Piano bootloader command.
    //!
    //! Each command is composed of a fixed length header of 16 bytes. This happens to be
    //! the size of a cipher block. Most commands will only require the header.
    //!
    //! But some commands, i.e. the "load data" command, may require additional arbitrary
    //! amounts of data. This data is packed into the N cipher blocks that immediately
    //! follow the command header. If the length of the data is not divisible by 16, then
    //! random (not zero!) pad bytes will be added.
    struct boot_command_t
    {
        uint8_t m_checksum; //!< Simple checksum over other command fields.
        uint8_t m_tag;      //!< Tag telling which command this is.
        uint16_t m_flags;   //!< Flags for this command.
        uint32_t m_address; //!< Target address.
        uint32_t m_count;   //!< Number of bytes on which to operate.
        uint32_t m_data;    //!< Additional data used by certain commands.
    };

#pragma pack()

    //! \brief Bootloader command tag constants.
    enum
    {
        ROM_NOP_CMD = 0x00,        //!< A no-op command.
        ROM_TAG_CMD = 0x01,        //!< Section tag command.
        ROM_LOAD_CMD = 0x02,       //!< Load data command.
        ROM_FILL_CMD = 0x03,       //!< Pattern fill command.
        ROM_JUMP_CMD = 0x04,       //!< Jump to address command.
        ROM_CALL_CMD = 0x05,       //!< Call function command.
        ROM_MODE_CMD = 0x06,       //!< Change boot mode command.
        ROM_ERASE_CMD = 0x07,      //!< Flash erase command.
        ROM_RESET_CMD = 0x08,      //!< Reset command.
        ROM_MEM_ENABLE_CMD = 0x09, //!< Memory Enable command.
        ROM_PROG_CMD = 0x0a        //!< Program persistent bits command.
    };

    //! \brief Flag field constants for #ROM_TAG_CMD.
    enum
    {
        ROM_SECTION_BOOTABLE = (1 << 0), //!< The section contains bootloader commands.
        ROM_SECTION_CLEARTEXT = (1 << 1), //!< The section is unencrypted. Applies only if the rest of the boot image is encrypted.
        ROM_LAST_TAG = (1 << 15)         //!< This tag command is the last one in the image.
    };

    //! \brief Flag field constants for #ROM_LOAD_CMD.
    enum
    {
        ROM_LOAD_DCD = (1 << 0) //!< Execute the DCD after loading completes.
    };

    //! \brief Flag field constants for #ROM_FILL_CMD.
    enum
    {
        ROM_FILL_BYTE = 0,      //!< Fill with byte sized pattern.
        ROM_FILL_HALF_WORD = 1, //!< Fill with half-word sized pattern.
        ROM_FILL_WORD = 2       //!< Fill with word sized pattern.
    };

    //! \brief Flag field constants for #ROM_JUMP_CMD and #ROM_CALL_CMD.
    enum
    {
        ROM_HAB_EXEC = (1 << 0),    //!< Changes jump or call command to a HAB jump or call.
        ROM_JUMP_SP_MASK = (1 << 1) //!< Initial stack pointer is specified for jump command.
    };

    //! \brief Flags for the #ROM_ERASE_CMD.
    enum
    {
        ROM_ERASE_ALL_MASK = (1 << 0),          //!< Erase all flash instead of just a range.
        ROM_ERASE_ALL_UNSECURE_MASK = (1 << 1), //!< Erase all unsecure (available on some Kinetis parts).
    };

    //! \brief Flags for the #ROM_PROG_CMD.
    enum
    {
        ROM_PROG_8BYTE_MASK = (1 << 0) //!< Program 8 bytes instead of 4 bytes.
    };

    //! \brief Memory ID flags field for ROM CMDs
    enum
    {
        ROM_MEM_DEVICE_ID_MASK = 0xff00, //!< Bit mask for mem controller ID.
        ROM_MEM_DEVICE_ID_SHIFT = 8,   //!< Bit position of mem controller ID.

        ROM_MEM_GROUP_ID_MASK = 0xf0,
        ROM_MEM_GROUP_ID_SHIFT = 4,
    };

//! @brief Bit mask for device ID.
#define DEVICE_ID_MASK 0xff
//! @brief Bit position of device ID.
#define DEVICE_ID_SHIFT 0
//! @brief Bit mask for group ID.
#define GROUP_ID_MASK 0xf00
//! @brief Bit position of group ID.
#define GROUP_ID_SHIFT 8

/*! @brief Construct a memory ID from a given group ID and device ID. */
#define MAKE_MEMORYID(group, device) ((((group) << GROUP_ID_SHIFT) & GROUP_ID_MASK) \
                                     | (((device) << DEVICE_ID_SHIFT) & DEVICE_ID_MASK))
/*! @brief Get group ID from a given memory ID. */
#define GROUPID(memoryId) (((memoryId) & GROUP_ID_MASK) >> GROUP_ID_SHIFT)

/*! @brief Get device ID from a given memory ID. */
#define DEVICEID(memoryId) (((memoryId) & DEVICE_ID_MASK ) >> DEVICE_ID_SHIFT)

    //! \brief Constants for Memory ID.
    enum
    {
        ROM_MEM_ID_Internal = 0, //!< Internal memoryID
        ROM_MEM_ID_QSPI = 1, //!< External QuadSpi controller.
        ROM_MEM_ID_IFR = 4, //!< IFR memory.
        ROM_MEM_ID_FLEXSPINOR = 9, // FlexSPI NOR FLASH.
        ROM_MEM_ID_FLASHEXECUTEONLY = 0x10, // Execute-only region on internal flash.

        ROM_MEM_ID_SPINAND = 0x101, // SPI NAND memory.
        ROM_MEM_ID_SPIEEPROM = 0x104, // SPI EEPROM memory.
        ROM_MEM_ID_I2CEEPROM = 0x105, // I2C EEPROM memory.
    };

    //! \brief Memory group definition
    enum 
    {
        ROM_MEM_GROUP_INTERNAL = 0, //!<  Kinetis internal 4G memory region.
        ROM_MEM_GROUP_EXTERNAL = 1, //!<  Kinetis external memory region.
    };

public:
    // Forward declaration.
    class Section;

    /*!
     * \brief Base class for objects that produce cipher blocks.
     */
    class CipherBlockGenerator
    {
    public:
        //! \name Cipher blocks
        //@{
        //! \brief Returns the total number of cipher blocks.
        //!
        //! The default implementation returns 0, indicating that no blocks are
        //! available.
        virtual unsigned getBlockCount() const { return 0; }
        //! \brief Returns the contents of up to \a maxCount cipher blocks.
        //!
        //! Up to \a maxCount cipher blocks are copied into the buffer pointed to by
        //! the \a data argument. This is only a request for \a maxCount blocks,
        //! the subclass implementation of this method is free to return any number
        //! of blocks from 0 up to \a maxCount. A return value of 0 indicates that
        //! no more blocks are available. The index of the first block to copy is
        //! held in the \a offset argument.
        //!
        //! \param offset Starting block number to copy. Zero means the first available block.
        //! \param maxCount Up to this number of blocks may be copied into \a data. Must be 1 or greater.
        //! \param data Buffer for outgoing cipher blocks. Must have enough room to hold
        //!		\a maxCount blocks.
        //!
        //! \return The number of cipher blocks copied into \a data.
        //! \retval 0 No more blocks are available and nothing was written to \a data.
        virtual unsigned getBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data) { return 0; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const {}
    };

    /*!
     * \brief Abstract base class for all bootloader commands.
     */
    class BootCommand : public CipherBlockGenerator
    {
    public:
        //! \brief Creates the correct subclass of BootCommand for the given raw data.
        static BootCommand *createFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

    public:
        //! \brief Default constructor.
        BootCommand()
            : CipherBlockGenerator()
        {
        }

        //! \brief Destructor.
        virtual ~BootCommand() {}
        //! \brief Read the command contents from raw data.
        //!
        //! The subclass implementations should validate the contents of the command, including
        //! the fields of the command header in the first block. It should be assumed that
        //! only the tag field was examined to determine which subclass of BootCommand
        //! should be created.
        //!
        //! \param blocks Pointer to the raw data blocks.
        //! \param count Number of blocks pointed to by \a blocks.
        //! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
        //!		by the command. Should be at least 1 for every command. This must not be NULL
        //!		on entry!
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed) = 0;

        //! \name Header
        //@{
        //! \brief Pure virtual method to return the tag value for this command.
        virtual uint8_t getTag() const = 0;

        //! \brief Pure virtual method to construct the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header) = 0;

        //! \brief Calculates the checksum for the given command header.
        virtual uint8_t calculateChecksum(const boot_command_t &header);
        //@}

        //! \name Cipher blocks
        //@{
        //! \brief Returns the total number of cipher blocks.
        virtual unsigned getBlockCount() const;

        //! \brief Returns the contents of up to \a maxCount cipher blocks.
        virtual unsigned getBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data);
        //@}

        //! \name Data blocks
        //@{
        //! \brief Returns the number of data cipher blocks that follow this command.
        //!
        //! The default implementation returns 0, indicating that no data blocks are
        //! available.
        virtual unsigned getDataBlockCount() const { return 0; }
        //! \brief Returns the contents of up to \a maxCount data blocks.
        //!
        //! Up to \a maxCount data blocks are copied into the buffer pointed to by
        //! the \a data argument. This is only a request for \a maxCount blocks,
        //! the subclass implementation of this method is free to return any number
        //! of blocks from 0 up to \a maxCount. A return value of 0 indicates that
        //! no more blocks are available. The index of the first block to copy is
        //! held in the \a offset argument.
        //!
        //! \param offset Starting block number to copy. Zero means the first available block.
        //! \param maxCount Up to this number of blocks may be copied into \a data. Must be 1 or greater.
        //! \param data Buffer for outgoing data blocks. Must have enough room to hold
        //!		\a maxCount blocks.
        //!
        //! \return The number of data blocks copied into \a data.
        //! \retval 0 No more blocks are available and nothing was written to \a data.
        virtual unsigned getDataBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data) { return 0; }
        //@}

    protected:
        //! The flag bit values for the \a whichFields parameter of validateHeader().
        enum
        {
            CMD_TAG_FIELD = 1,
            CMD_FLAGS_FIELD = 2,
            CMD_ADDRESS_FIELD = 4,
            CMD_COUNT_FIELD = 8,
            CMD_DATA_FIELD = 16
        };

        //! \brief
        void validateHeader(const boot_command_t *modelHeader, const boot_command_t *testHeader, unsigned whichFields);
    };

    /*!
     * \brief No operation bootloader command.
     */
    class NopCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        NopCommand()
            : BootCommand()
        {
        }

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_NOP_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;
    };

    /*!
     * \brief Section tag bootloader command.
     */
    class TagCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        TagCommand()
            : BootCommand()
        {
            m_hmacEntries=1;
        }

        //! \brief Constructor taking a section object.
        TagCommand(const Section &section);

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_TAG_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \name Field accessors
        //@{
        inline void setSectionIdentifier(uint32_t identifier) { m_sectionIdentifier = identifier; }
        inline void setSectionLength(uint32_t length) { m_sectionLength = length; }
        inline void setSectionFlags(uint32_t flags) { m_sectionFlags = flags; }
        inline void setLast(bool isLast) { m_isLast = isLast; m_sectionFlags |= ROM_LAST_TAG; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        uint32_t m_sectionIdentifier; //!< Unique identifier for the section containing this command.
        uint32_t m_sectionLength;     //!< Number of cipher blocks this section occupies.
        uint16_t m_sectionFlags;      //!< Flags pertaining to this section.
        bool m_isLast;                //!< Is this the last tag command?
        uint32_t m_hmacEntries;       //!< Number of hmac entries
    };

    /*!
     * \brief Load data bootloader command.
     */
    class LoadCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        LoadCommand();

        //! \brief Constructor.
        LoadCommand(uint32_t address, const uint8_t *data, uint32_t length);

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_LOAD_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);

        //! \brief Sets the load-dcd flag.
        inline void setDCD(bool isDCD) { m_loadDCD = isDCD; }
        //@}

        //! \name Address
        //@{
        inline void setLoadAddress(uint32_t address) { m_address = address; }
        inline uint32_t getLoadAddress() const { return m_address; }
        //@}

        //! \name Load data
        //@{
        //! \brief Set the data for the command to load.
        void setData(const uint8_t *data, uint32_t length);

        inline uint8_t *getData() { return m_data; }
        inline const uint8_t *getData() const { return m_data; }
        inline uint32_t getLength() const { return m_length; }
        //@}

        //! \name Data blocks
        //@{
        //! \brief Returns the number of data cipher blocks that follow this command.
        virtual unsigned getDataBlockCount() const;

        //! \brief Returns the contents of up to \a maxCount data blocks.
        virtual unsigned getDataBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data);
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

        //! \name Memory space id.
        //@{
        void setMemoryId(uint32_t id) { m_memoryId = id; }
        uint32_t getMemoryId() const { return m_memoryId; }
        //@}

    protected:
        smart_array_ptr<uint8_t> m_data; //!< Pointer to data to load.
        uint8_t m_padding[15];           //!< Up to 15 pad bytes may be required.
        unsigned m_padCount;             //!< Number of pad bytes.
        uint32_t m_length;               //!< Number of bytes to load.
        uint32_t m_address;              //!< Address to which data will be loaded.
        bool m_loadDCD;                  //!< Whether to execute the DCD after loading.
        uint32_t m_memoryId;             //!< Memory device ID.

        void fillPadding();
        uint32_t calculateCRC() const;
    };

    /*!
     * \brief Pattern fill bootloader command.
     */
    class FillCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        FillCommand();

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_FILL_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \name Address range
        //@{
        inline void setAddress(uint32_t address) { m_address = address; };
        inline uint32_t getAddress() const { return m_address; }
        inline void setFillCount(uint32_t count) { m_count = count; }
        inline uint32_t getFillCount() const { return m_count; }
        //@}

        //! \name Pattern
        //@{
        void setPattern(uint8_t pattern);
        void setPattern(uint16_t pattern);
        void setPattern(uint32_t pattern);

        inline uint32_t getPattern() const { return m_pattern; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        uint32_t m_address; //!< Fill start address.
        uint32_t m_count;   //!< Number of bytes to fill.
        uint32_t m_pattern; //!< Fill pattern.
    };

    /*!
     * \brief Change boot mode bootloader command.
     */
    class ModeCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        ModeCommand()
            : BootCommand()
            , m_mode(0)
        {
        }

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_MODE_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \name Boot mode
        //@{
        inline void setBootMode(uint32_t mode) { m_mode = mode; }
        inline uint32_t getBootMode() const { return m_mode; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        uint32_t m_mode; //!< New boot mode.
    };

    /*!
     * \brief Erase bootloader command.
     */
    class EraseCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        EraseCommand()
            : BootCommand()
            , m_doEraseAll(false)
            , m_doEraseAllUnsecure(false)
            , m_startAddress(0)
            , m_byteCount(0)
            , m_memoryId(0)
        {
        }

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_ERASE_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \name Erase all
        //@{
        void setEraseAll(bool doEraseAll) { m_doEraseAll = doEraseAll; }
        bool getEraseAll() const { return m_doEraseAll; }
        //@}

        //! \name Erase all unsecure
        //@{
        void setEraseAllUnsecure(bool doEraseAllUnsecure) { m_doEraseAllUnsecure = doEraseAllUnsecure; }
        bool getEraseAllUnsecure() const { return m_doEraseAllUnsecure; }
        //@}

        //! \name Erase address range
        //@{
        void setAddressRange(uint32_t startAddress, uint32_t count);
        void getAddressRange(uint32_t *startAddress, uint32_t *count) const;
        //@}

        //! \name Erase memory ID
        //@{
        void setMemoryId(uint32_t memId) { m_memoryId = memId; }
        uint32_t getMemoryId() const { return m_memoryId; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        bool m_doEraseAll;
        bool m_doEraseAllUnsecure;
        uint32_t m_startAddress;
        uint32_t m_byteCount;
        uint32_t m_memoryId;
    };

    /*!
    * \brief Program persistent bits bootloader command.
    */
    class ProgramCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        ProgramCommand()
            : BootCommand()
            , m_isEightByte(false)
            , m_index(0)
            , m_dataWord1(0)
            , m_dataWord2(0)
            , m_memoryId(0)
        {
        }

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_PROG_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \name Do 8-byte write.
        //@{
        void setIsEightByte(bool doIt) { m_isEightByte = doIt; }
        bool isEightByte() const { return m_isEightByte; }
        //@}

        //! \name Memory space id.
        //@{
        void setMemoryId(uint32_t id) { m_memoryId = id; }
        uint32_t getMemoryId() const { return m_memoryId; }
        //@}

        //! \name Persistent bit index
        //@{
        void setIndex(uint32_t index) { m_index = index; }
        uint32_t getIndex() const { return m_index; }
        //@}

        //! \name Data bytes 0-3
        //@{
        void setDataWord1(uint32_t dataWord) { m_dataWord1 = dataWord; }
        uint32_t getDataWord1() const { return m_dataWord1; }
        //@}

        //! \name Data bytes 4-7
        //@{
        void setDataWord2(uint32_t dataWord) { m_dataWord2 = dataWord; }
        uint32_t getDataWord2() const { return m_dataWord2; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        bool m_isEightByte;   //!< True if doing 8-byte write.
        uint32_t m_index;     //!< Index to write to.
        uint32_t m_dataWord1; //!< First 4 bytes to write.
        uint32_t m_dataWord2; //!< Second 4 bytes to write, valid if m_doEightByte is true.
        uint32_t m_memoryId;  //!< ID of memory space, e.g. 1=IFR0.
    };

    /*!
  * \brief Jump to address bootloader command.
  */
    class JumpCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        JumpCommand()
            : BootCommand()
            , m_address(0)
            , m_argument(0)
            , m_isHAB(false)
            , m_ivtSize(0)
            , m_stackPointer(0)
            , m_isStackPointerSet(false)
        {
        }

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_JUMP_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \name Accessors
        //@{
        inline void setAddress(uint32_t address) { m_address = address; }
        inline uint32_t getAddress() const { return m_address; }
        inline void setArgument(uint32_t argument) { m_argument = argument; }
        inline uint32_t getArgument() const { return m_argument; }
        inline void setIsHAB(bool isHAB) { m_isHAB = isHAB; }
        inline bool isHAB() const { return m_isHAB; }
        inline void setIVTSize(uint32_t ivtSize) { m_ivtSize = ivtSize; }
        inline uint32_t getIVTSize() const { return m_ivtSize; }
        inline void setStackPointer(uint32_t sp) { m_stackPointer = sp; }
        inline uint32_t getStackPointer() { return m_stackPointer; }
        inline void setIsStackPointerSet(bool isSP) { m_isStackPointerSet = isSP; }
        inline bool IsStackPointerSet() const { return m_isStackPointerSet; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        uint32_t m_address;      //!< Address of the code to execute.
        uint32_t m_argument;     //!< Sole argument to pass to code.
        uint32_t m_stackPointer; //!< Stack pointer for "jump sp" command.
        bool m_isHAB; //!< Whether this jump/call is a special HAB jump/call. When this flag is set, m_address becomes
                      //!the IVT address and m_ivtSize is the IVT size.
        bool m_isStackPointerSet; //! Whether this jump call has an initial stack pointer set.
        uint32_t m_ivtSize;       //!< Size of the IVT for a HAB jump/call.
    };

    /*!
     * \brief Call function bootloader command.
     */
    class CallCommand : public JumpCommand
    {
    public:
        //! \brief Default constructor.
        CallCommand()
            : JumpCommand()
        {
        }

        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_CALL_CMD; }
        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;
    };

    /*!
    * \brief Reset target bootloader command.
    */
    class ResetCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        ResetCommand()
            : BootCommand()
        {
        }

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_RESET_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
    };

    /*!
    * \brief Memory Enable bootloader command.
    */
    class MemEnableCommand : public BootCommand
    {
    public:
        //! \brief Default constructor.
        MemEnableCommand()
            : BootCommand()
            , m_startAddress(0)
            , m_byteCount(0)
            , m_memoryId(0)
        {
        }

        //! \brief Read the command contents from raw data.
        virtual void initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed);

        //! \name Header
        //@{
        //! \brief Returns the tag value for this command.
        virtual uint8_t getTag() const { return ROM_MEM_ENABLE_CMD; }
        //! \brief Constructs the header for this boot command.
        virtual void fillCommandHeader(boot_command_t &header);
        //@}

        //! \name Memory enable address range
        //@{
        void setAddressRange(uint32_t startAddress, uint32_t count);
        void getAddressRange(uint32_t *startAddress, uint32_t *count) const;
        //@}

        //! \name Memory enable controller ID
        //@{
        void setMemoryId(uint32_t id) { m_memoryId = id; }
        uint32_t getMemoryId() const { return m_memoryId; }
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        uint32_t m_startAddress;
        uint32_t m_byteCount;
        uint32_t m_memoryId;
    };

    /*!
  * \brief Base class for a section of an Encore boot image.
  *
  * Provides methods to manage the unique identifier that all sections have, and
  * to set the boot image object which owns the section. There are also virtual
  * methods to get header flags and fill in the header used in the section
  * table. Subclasses must implement at least fillSectionHeader().
  */
    class Section : public CipherBlockGenerator
    {
    public:
        //! \brief Default constructor.
        Section()
            : CipherBlockGenerator()
            , m_identifier(0)
            , m_image(0)
            , m_alignment(BOOT_IMAGE_MINIMUM_SECTION_ALIGNMENT)
            , m_flags(0)
            , m_leaveUnencrypted(false)
            , m_hmacEntries(1)
        {
        }

        //! \brief Constructor taking the unique identifier for this section.
        Section(uint32_t identifier)
            : CipherBlockGenerator()
            , m_identifier(identifier)
            , m_image(0)
            , m_alignment(BOOT_IMAGE_MINIMUM_SECTION_ALIGNMENT)
            , m_flags(0)
            , m_leaveUnencrypted(false)
            , m_hmacEntries(1)
        {
        }

        //! \name Identifier
        //@{
        inline void setIdentifier(uint32_t identifier) { m_identifier = identifier; }
        inline uint32_t getIdentifier() const { return m_identifier; }
        //@}

        //! \name Header
        //@{
        //! \brief Sets explicit flags for this section.
        virtual void setFlags(uint32_t flags) { m_flags = flags; }
        //! \brief Returns the flags for this section.
        //!
        //! The return value consists of the flags set with setFlags() possibly or-ed
        //! with #ROM_SECTION_CLEARTEXT if the section has been set to be left
        //! unencrypted.
        virtual uint32_t getFlags() const { return m_flags | (m_leaveUnencrypted ? ROM_SECTION_CLEARTEXT : 0); }
        //! \brief Pure virtual method to construct the header for this section.
        virtual void fillSectionHeader(section_header_t &header);
        //@}

        //! \name Owner image
        //@{
        //! \brief Called when the section is added to an image.
        void setImage(SB2Image *image) { m_image = image; }
        //! \brief Returns a pointer to the image that this section belongs to.
        SB2Image *getImage() const { return m_image; }
        //@}

        //! \name Alignment
        //@{
        //! \brief Sets the required alignment in the output file for this section.
        void setAlignment(unsigned alignment);

        //! \brief Returns the current alignment, the minimum of which will be 16.
        unsigned getAlignment() const { return m_alignment; }
        //! \brief Computes padding amount for alignment requirement.
        unsigned getPadBlockCountForOffset(unsigned offset);
        //@}

        //! \name Leave unencrypted flag
        //@{
        //! \brief Sets whether the section will be left unencrypted.
        void setLeaveUnencrypted(unsigned flag) { m_leaveUnencrypted = flag; }
        //! \brief Returns true if the section will remain unencrypted.
        bool getLeaveUnencrypted() const { return m_leaveUnencrypted; }
        //@}

        //! \name hmac Entries flag
        //@{
        //! \brief Sets whether the section will be left unencrypted.
        void setHmacEntries(uint32_t hmacEntries) { m_hmacEntries = hmacEntries; }
        //! \brief Returns true if the section will remain unencrypted.
        uint32_t getHmacEntries() const { return m_hmacEntries; }
        //@}

    protected:
        uint32_t m_identifier;    //!< Unique identifier for this section.
        SB2Image *m_image; //!< The image to which this section belongs.
        unsigned m_alignment;     //!< Alignment requirement for the start of this section.
        uint32_t m_flags;         //!< Section flags set by the user.
        bool m_leaveUnencrypted;  //!< Set to true to prevent this section from being encrypted.
        uint32_t m_hmacEntries;   //!< Number of hmac entries for the section
    };

    /*!
     * \brief A bootable section of an Encore boot image.
     */
    class BootSection : public Section
    {
    public:
        typedef std::list<BootCommand *> command_list_t;
        typedef command_list_t::iterator iterator_t;
        typedef command_list_t::const_iterator const_iterator_t;

    public:
        //! \brief Default constructor.
        BootSection()
            : Section()
        {
        }

        //! \brief Constructor taking the unique identifier for this section.
        BootSection(uint32_t identifier)
            : Section(identifier)
        {
        }

        //! \brief Destructor.
        virtual ~BootSection();

        //! \brief Load the section from raw data.
        virtual void fillFromData(const cipher_block_t *blocks, unsigned count);

        //! \name Header
        //@{
        //! \brief Returns the flags for this section.
        virtual uint32_t getFlags() const { return Section::getFlags() | ROM_SECTION_BOOTABLE; }
        //@}

        //! \name Commands
        //@{
        //! \brief Append a new command to the section.
        //!
        //! The section takes ownership of the command and will delete it when
        //! the section is destroyed.
        void addCommand(BootCommand *command) { m_commands.push_back(command); }
        //! \brief Returns the number of commands in this section, excluding the tag command.
        unsigned getCommandCount() const { return (unsigned)m_commands.size(); }
        iterator_t begin() { return m_commands.begin(); }
        iterator_t end() { return m_commands.end(); }
        const_iterator_t begin() const { return m_commands.begin(); }
        const_iterator_t end() const { return m_commands.end(); }
        //@}

        //! \name Cipher blocks
        //@{
        //! \brief Returns the total number of cipher blocks occupied by this section.
        virtual unsigned getBlockCount() const;

        //! \brief Returns the contents of up to \a maxCount cipher blocks.
        virtual unsigned getBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data);
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        command_list_t m_commands; //!< Commands held in this section.

    protected:
        //! \brief Remove all commands from the section.
        void deleteCommands();
    };

    /*!
     * \brief A non-bootable section of an Encore boot image.
     */
    class DataSection : public Section
    {
    public:
        //! \brief Default constructor.
        DataSection()
            : Section()
            , m_data()
            , m_length(0)
        {
        }

        //! \brief Constructor taking the unique identifier for this section.
        DataSection(uint32_t identifier)
            : Section(identifier)
            , m_data()
            , m_length(0)
        {
        }

        //! \brief Set the data section's contents.
        void setData(const uint8_t *data, unsigned length);

        //! \brief Set the data section's contents without copying \a data.
        void setDataNoCopy(const uint8_t *data, unsigned length);

        //! \name Cipher blocks
        //@{
        //! \brief Returns the total number of cipher blocks occupied by this section.
        virtual unsigned getBlockCount() const;

        //! \brief Returns the contents of up to \a maxCount cipher blocks.
        virtual unsigned getBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data);
        //@}

        //! \brief Print out a string representation of the object.
        virtual void debugPrint() const;

    protected:
        smart_array_ptr<uint8_t> m_data; //!< The section's contents.
        unsigned m_length;               //!< Number of bytes of data.
    };

public:
    typedef std::list<Section *> section_list_t;                     //!< List of image sections.
    typedef section_list_t::iterator section_iterator_t;             //!< Iterator over sections.
    typedef section_list_t::const_iterator const_section_iterator_t; //!< Const iterator over sections.

    typedef std::vector<AES256Key> key_list_t;               //!< List of KEKs.
    typedef key_list_t::iterator key_iterator_t;             //!< Iterator over KEKs.
    typedef key_list_t::const_iterator const_key_iterator_t; //!< Const iterator over KEKs.

public:
    //! \brief Default constructor.
    SB2Image();

    //! \brief Destructor.
    virtual ~SB2Image();

    //! \name Sections
    //@{
    void addSection(Section *newSection);
    inline unsigned sectionCount() const { return (unsigned)m_sections.size(); }
    inline section_iterator_t beginSection() { return m_sections.begin(); }
    inline section_iterator_t endSection() { return m_sections.end(); }
    inline const_section_iterator_t beginSection() const { return m_sections.begin(); }
    inline const_section_iterator_t endSection() const { return m_sections.end(); }
    section_iterator_t findSection(Section *section);

    //! \brief Calculates the starting block number for the given section.
    uint32_t getSectionOffset(Section *section);
    //@}

    //! \name Encryption keys
    //@{
    inline void addKey(const AES256Key &newKey) { m_keys.push_back(newKey); }
    inline void setPrivKeyPath(const char *pk) { m_privKeyPath = pk; }
    inline void addHashFileName(const char *hf) { (hf == NULL) ? m_hashFilePath = "hash.bin" : m_hashFilePath = hf; }
    inline unsigned keyCount() const { return (unsigned)m_keys.size(); }
    inline key_iterator_t beginKeys() { return m_keys.begin(); }
    inline key_iterator_t endKeys() { return m_keys.end(); }
    inline const_key_iterator_t beginKeys() const { return m_keys.begin(); }
    inline const_key_iterator_t endKeys() const { return m_keys.end(); }
    
    inline void addCerts(const string_vector_t &certFilePaths) { m_certFilePaths = certFilePaths; }
    inline void addRootKeyCerts(const string_vector_t &rootKeyCertFilePaths) { m_rootKeyCertFilePaths = rootKeyCertFilePaths; }
    inline void setDek(const AES256Key dek) {m_dek = dek;}
    inline void setMacKey(const AES256Key mac) { m_macKey = mac; }

   //! \brief The image is encrypted if there is at least one key.
    inline bool isEncrypted() const { return TRUE; }
    inline bool isSigned() const { return (getFlags() & ROM_SIGNED); }
    inline uint8_t getCipherMode() const { return ((getFlags() & ROM_CIPHER_MODE) >> CIPHER_MODE_SHIFT); }
    //@}

    //! \name Versions
    //@{
    virtual void setProductVersion(const version_t &version);
    virtual void setComponentVersion(const version_t &version);
    //@}

    //! \name Flags
    //@{
    inline void setFlags(uint16_t flags) { m_headerFlags = flags; }
    inline uint32_t getFlags() const { return m_headerFlags; }
    //@}

    //! \brief drive Tag for SB2 image is not available.
    virtual void setDriveTag(uint16_t) { ; }

    //! \brief Specify the build number to be set in the output file header.
    virtual void setBuildNumber(uint16_t build) { m_buildNumber = build; }

    //! \brief Calculates the total number of cipher blocks the image consumes.
    uint32_t getImageSize();

    //! \brief Returns the preferred ".sb" extension for Encore boot images.
    virtual std::string getFileExtension() const { return ".sb2"; }
    //! \name Output
    //@{
    //! \brief Write the boot image to an output stream.
    virtual void writeToStream(std::ostream &stream);
    //@}

    //! \brief Print out a string representation of the object.
    virtual void debugPrint() const;

protected:
    uint16_t m_headerFlags;       //!< Flags field in the boot image header.
    version_t m_productVersion;   //!< Product version.
    version_t m_componentVersion; //!< Component version.
    uint16_t m_buildNumber;       //!< Build Number for this boot image.
    section_list_t m_sections;    //!< Sections contained in this image.
    key_list_t m_keys;            //!< List of key encryption keys. If empty, the image is unencrypted.
    AES256Key m_dek;              //!< Session key we're using.
    AES256Key m_macKey;           //!< MAC key we're using.
    const char * m_privKeyPath;   //!< Private key path used in signing
    const char * m_hashFilePath;  //!< Output path to hash file
    string_vector_t m_certFilePaths;  //!< Paths to dek files.
    uint8_t m_digestHmac[32];     //!< digest HMAC
    certificate_block_header_t m_certBlockHeader; //!< Certificate block header
    uint8_t m_keyBlob[((64 * 9) / 8) + 8]; //!< keyBlob
    SB2Image::DataSection * m_signDataSection;  //!< Sign certificates data section
    cert_table_t m_certTable;  //! Certificate table
    rkh_table m_rkhTable;      //! Root key hashes table
    string_vector_t m_rootKeyCertFilePaths;  //!< Paths to root key certificate files.
    
    void prepareImageHeader(sb2_header_t &header);
    uint64_t getTimestamp();
    Section *findFirstBootableSection();
    unsigned getPadBlockCountForSection(Section *section, unsigned offset);
    void prepareCertBlockHeader();
    void prepareSignDataSection();
    uint32_t getKeyBlobBlock();
    uint32_t getKeyBlobBlockCount();
    uint16_t getMaxSectionMacCount();
    uint32_t getFirstBootTagBlock();
};
}; // namespace elftosb

#endif // _SB2Image_h_
