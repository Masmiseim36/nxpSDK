/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_SBSourceFile_h_)
#define _SBSourceFile_h_

#include "blfwk/SourceFile.h"
#include "blfwk/StELFFile.h"
#include "blfwk/smart_ptr.h"
#include "blfwk/DataSource.h"
#include "blfwk/DataTarget.h"

namespace blfwk
{
/*!
 * \brief SB Format (SB) source file.
 */
class SBSourceFile : public BinarySourceFile
{
public:
    //! \brief Default constructor.
    SBSourceFile(const std::string &path);

    //! \brief Destructor.
    virtual ~SBSourceFile();

    //! \brief Identifies whether the stream contains an SB file.
    static bool isSBFile(std::istream &stream);

    //! \name Data source creation
    //@{
    //! \brief Creates an unmapped data source from the entire file.
    virtual DataSource *createDataSource();
    //@}

protected:
    //! An AES-128 cipher block is 16 bytes.
    typedef uint8_t cipher_block_t[16];

    //! A SHA-1 digest is 160 bits, or 20 bytes.
    typedef uint8_t sha1_digest_t[20];

    //! Unique identifier type for a section.
    typedef uint32_t section_id_t;

// All of these structures are packed to byte alignment in order to match
// the structure on disk.
#pragma pack(1)

    //! Same version struct used for 3600 boot image.
    struct version_t
    {
        uint16_t m_major;
        uint16_t m_pad0;
        uint16_t m_minor;
        uint16_t m_pad1;
        uint16_t m_revision;
        uint16_t m_pad2;
    };

    //! \brief Header for the entire boot image.
    //!
    //! Fields of this header are arranged so that those used by the bootloader ROM all come
    //! first. They are also set up so that all fields are not split across cipher block
    //! boundaries. The fields not used by the bootloader are not subject to this
    //! restraint.
    //!
    //! Image header size is always a round number of cipher blocks. The same also applies to
    //! the boot image itself. The padding, held in #blfwk::EncoreBootImage::boot_image_header_t::m_padding0
    //! and #blfwk::EncoreBootImage::boot_image_header_t::m_padding1 is filled with random bytes.
    //!
    //! The DEK dictionary, section table, and each section data region must all start on
    //! cipher block boundaries.
    //!
    //! This header is not encrypted in the image file.
    //!
    //! The m_digest field contains a SHA-1 digest of the fields of the header that follow it.
    //! It is the first field in the header so it doesn't change position or split the header
    //! in two if fields are added to the header.
    struct boot_image_header_t
    {
        union
        {
            sha1_digest_t m_digest; //!< SHA-1 digest of image header. Also used as the crypto IV.
            struct
            {
                cipher_block_t m_iv; //!< The first 16 bytes of the digest form the initialization vector.
                uint8_t m_extra[4];  //!< The leftover top four bytes of the SHA-1 digest.
            };
        };
        uint8_t m_signature[4];       //!< 'STMP', see #ROM_IMAGE_HEADER_SIGNATURE.
        uint8_t m_majorVersion;       //!< Major version for the image format, see #ROM_BOOT_IMAGE_MAJOR_VERSION.
        uint8_t m_minorVersion;       //!< Minor version of the boot image format, see #ROM_BOOT_IMAGE_MINOR_VERSION.
        uint16_t m_flags;             //!< Flags or options associated with the entire image.
        uint32_t m_imageBlocks;       //!< Size of entire image in blocks.
        uint32_t m_firstBootTagBlock; //!< Offset from start of file to the first boot tag, in blocks.
        section_id_t m_firstBootableSectionID; //!< ID of section to start booting from.
        uint16_t m_keyCount;                   //!< Number of entries in DEK dictionary.
        uint16_t m_keyDictionaryBlock;         //!< Starting block number for the key dictionary.
        uint16_t m_headerBlocks;               //!< Size of this header, including this size word, in blocks.
        uint16_t m_sectionCount;               //!< Number of section headers in this table.
        uint16_t m_sectionHeaderSize;          //!< Size in blocks of a section header.
        uint8_t m_padding0[2];                 //!< Padding to align #m_timestamp to long word.
        uint8_t m_signature2[4];      //!< Second signature to distinguish this .sb format from the 36xx format, see
                                      //!#ROM_IMAGE_HEADER_SIGNATURE2.
        uint64_t m_timestamp;         //!< Timestamp when image was generated in microseconds since 1-1-2000.
        version_t m_productVersion;   //!< Product version.
        version_t m_componentVersion; //!< Component version.
        uint16_t m_driveTag;          //!< Drive tag for the system drive which this boot image belongs to.
        uint8_t m_padding1[6];        //!< Padding to round up to next cipher block.
    };

#pragma pack()

public:
    /*!
     * \brief Simple exception thrown to indicate an error in the input SB file format.
     */
    class SBFileException : public std::runtime_error
    {
    public:
        //! \brief Default constructor.
        SBFileException(const std::string &inMessage)
            : std::runtime_error(inMessage)
        {
        }
    };
};

}; // namespace blfwk

#endif // _SBSourceFile_h_
