/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include <algorithm>
#include "string.h"
#include "blfwk/stdafx.h"
#include "blfwk/EndianUtilities.h"
#include "blfwk/GHSSecInfo.h"
#include "blfwk/Logging.h"
#include "blfwk/SBSourceFile.h"

using namespace blfwk;

SBSourceFile::SBSourceFile(const std::string &path)
    : BinarySourceFile(path, kSBSourceFile)
{
}

SBSourceFile::~SBSourceFile()
{
}

bool SBSourceFile::isSBFile(std::istream &stream)
{
    try
    {
        boot_image_header_t header; //!< Header from the SB boot image.

        // readImageHeader
        // seek to beginning of the stream/file and read the plaintext header
        stream.seekg(0, std::ios_base::beg);
        if (stream.read((char *)&header, sizeof(header)).bad())
        {
            throw SBFileException("failed to read SB image header");
        }

        header.m_flags = ENDIAN_LITTLE_TO_HOST_U16(header.m_flags);
        header.m_imageBlocks = ENDIAN_LITTLE_TO_HOST_U32(header.m_imageBlocks);
        header.m_firstBootTagBlock = ENDIAN_LITTLE_TO_HOST_U32(header.m_firstBootTagBlock);
        header.m_firstBootableSectionID = ENDIAN_LITTLE_TO_HOST_U32(header.m_firstBootableSectionID);
        header.m_keyCount = ENDIAN_LITTLE_TO_HOST_U16(header.m_keyCount);
        header.m_keyDictionaryBlock = ENDIAN_LITTLE_TO_HOST_U16(header.m_keyDictionaryBlock);
        header.m_headerBlocks = ENDIAN_LITTLE_TO_HOST_U16(header.m_headerBlocks);
        header.m_sectionCount = ENDIAN_LITTLE_TO_HOST_U16(header.m_sectionCount);
        header.m_sectionHeaderSize = ENDIAN_LITTLE_TO_HOST_U16(header.m_sectionHeaderSize);
        header.m_timestamp = ENDIAN_LITTLE_TO_HOST_U64(header.m_timestamp);

        // check header signature 1
        if (header.m_signature[0] != 'S' || header.m_signature[1] != 'T' || header.m_signature[2] != 'M' ||
            header.m_signature[3] != 'P')
        {
            throw SBFileException("invalid SB signature 1");
        }

        // check header signature 2 for version 1.1 and greater
        if ((header.m_majorVersion > 1 || (header.m_majorVersion == 1 && header.m_minorVersion >= 1)) &&
            (header.m_signature2[0] != 's' || header.m_signature2[1] != 'g' || header.m_signature2[2] != 't' ||
             header.m_signature2[3] != 'l'))
        {
            Log::log(Logger::kWarning, "warning: invalid SB signature 2\n");
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

DataSource *SBSourceFile::createDataSource()
{
    throw std::runtime_error("SBSourceFile::createDataSource() has not been implemented.");
}
