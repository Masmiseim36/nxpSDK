/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include "blfwk/SourceFile.h"
#include "blfwk/SearchPath.h"
#include "blfwk/ELFSourceFile.h"
#include "blfwk/EndianUtilities.h"
#include "blfwk/SRecordSourceFile.h"
#include "blfwk/SBSourceFile.h"
#include "blfwk/IntelHexSourceFile.h"
#include "blfwk/format_string.h"

using namespace blfwk;

//! The supported file types are currently:
//!     - ELF files
//!     - Motorola S-record files
//!     - Binary files
//!     - SB files
//!     - Intel Hex files
//!
//! Any file that is not picked up by the other subclasses will result in a
//! an instance of BinaryDataFile.
//!
//! \return An instance of the correct subclass of SourceFile for the given path.
//!
//! \exception std::runtime_error Thrown if the file cannot be opened.
//!
//! \see blfwk::ELFSourceFile
//! \see blfwk::SRecordSourceFile
//! \see blfwk::BinarySourceFile
//! \see blfwk::SBSourceFile
//! \see blfwk::IntelHexSourceFile
SourceFile *SourceFile::openFile(const std::string &path)
{
    // Search for file using search paths
    std::string actualPath;
    bool found = PathSearcher::getGlobalSearcher().search(path, PathSearcher::kFindFile, true, actualPath);
    if (!found)
    {
        throw std::runtime_error(format_string("unable to find file %s\n", path.c_str()));
    }

    std::ifstream testStream(actualPath.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!testStream.is_open())
    {
        throw std::runtime_error(format_string("failed to open file: %s", actualPath.c_str()));
    }

    // catch exceptions so we can close the file stream
    try
    {
        if (ELFSourceFile::isELFFile(testStream))
        {
            testStream.close();
            return new ELFSourceFile(actualPath);
        }
        else if (SRecordSourceFile::isSRecordFile(testStream))
        {
            testStream.close();
            return new SRecordSourceFile(actualPath);
        }
        else if (SBSourceFile::isSBFile(testStream))
        {
            testStream.close();
            return new SBSourceFile(actualPath);
        }
        else if (IntelHexSourceFile::isIntelHexFile(testStream))
        {
            testStream.close();
            return new IntelHexSourceFile(actualPath);
        }

        // treat it as a binary file
        testStream.close();
        return new BinarySourceFile(actualPath);
    }
    catch (...)
    {
        testStream.close();
        throw;
    }
}

SourceFile::SourceFile(const std::string &path, source_file_t filetype)
    : m_path(path)
    , m_filetype(filetype)
    , m_stream()
{
    // Initialize m_size
    open();
    assert(m_stream);
    m_stream->seekg(0, std::ios_base::end);
    m_size = (unsigned)m_stream->tellg();
    close();
}

//! The file is closed if it had been left opened.
//!
SourceFile::~SourceFile()
{
    if (isOpen())
    {
        m_stream->close();
    }
}

//! \exception std::runtime_error Raised if the file could not be opened successfully.
void SourceFile::open()
{
    assert(!isOpen());
    m_stream = new std::ifstream(m_path.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!m_stream->is_open())
    {
        throw std::runtime_error(format_string("failed to open file: %s", m_path.c_str()));
    }
}

void SourceFile::close()
{
    assert(isOpen());

    m_stream->close();
    m_stream.safe_delete();
}

//! If the file does not support named sections, or if there is not a
//! section with the given name, this method may return NULL.
//!
//! This method is just a small wrapper that creates an
//! FixedMatcher string matcher instance and uses the createDataSource()
//! that takes a reference to a StringMatcher.
DataSource *SourceFile::createDataSource(const std::string &section)
{
    FixedMatcher matcher(section);
    return createDataSource(matcher);
}

DataTarget *SourceFile::createDataTargetForEntryPoint()
{
    if (!hasEntryPoint())
    {
        return NULL;
    }

    return new ConstantDataTarget(getEntryPointAddress());
}

BinarySourceFile::BinarySourceFile(const std::string &path, source_file_t sourceFileType)
    : SourceFile(path, sourceFileType)
    , m_entry_point(0xffffffff)
    , m_stack_pointer(0xffffffff)
{
}

void BinarySourceFile::guessEntryPointAndStackPointer()
{
    open();

    uint32_t data[] = { 0xffffffff, 0xffffffff };

    // seek to beginning of the stream/file and read the plaintext header
    m_stream->seekg(0, std::ios_base::beg);
    if (m_stream->read((char *)&data, sizeof(data)).bad())
    {
        close();
        throw std::runtime_error(format_string("failed to read image header from file: %s", m_path.c_str()));
    }

    close();

    m_stack_pointer = ENDIAN_LITTLE_TO_HOST_U32(data[0]);
    m_entry_point = ENDIAN_LITTLE_TO_HOST_U32(data[1]);
}

DataSource *BinarySourceFile::createDataSource()
{
    std::istream *fileStream = getStream();
    assert(fileStream);

    // get stream size
    fileStream->seekg(0, std::ios_base::end);
    int length = (int)fileStream->tellg();

    // allocate buffer
    smart_array_ptr<uint8_t> data = new uint8_t[length];

    // read entire file into the buffer
    fileStream->seekg(0, std::ios_base::beg);
    if (fileStream->read((char *)data.get(), length).bad())
    {
        throw std::runtime_error(format_string("unexpected end of file: %s", m_path.c_str()));
    }

    // create the data source. the buffer is copied, so we can dispose of it.
    return new UnmappedDataSource(data, length);
}
