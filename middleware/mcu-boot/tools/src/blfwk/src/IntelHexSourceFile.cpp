/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <string.h>
#include "blfwk/smart_ptr.h"
#include "blfwk/IntelHexSourceFile.h"
#include "blfwk/Logging.h"

enum
{
    //! Size in bytes of the buffer used to collect Intel Hex data records
    //! before adding them to the executable image.
    //! The address field in each record is 2 bytes, so set the buffer to 64KB.
    COLLECTION_BUFFER_SIZE = 64 * 1024
};

using namespace blfwk;

IntelHexSourceFile::IntelHexSourceFile(const std::string &path)
    : SourceFile(path, kIntelHexSourceFile)
    , m_file(NULL)
    , m_image(0)
    , m_hasEntryRecord(false)
{
    memset(&m_entryRecord, 0, sizeof(m_entryRecord));
}

bool IntelHexSourceFile::isIntelHexFile(std::istream &stream)
{
    StIntelHexFile hex(stream);
    return hex.isIntelHexFile();
}

void IntelHexSourceFile::open()
{
    SourceFile::open();

    // create file parser and examine file
    m_file = new StIntelHexFile(*m_stream);
    m_file->parse();

    // build an image of the file
    m_image = new StExecutableImage();
    buildMemoryImage();

    // dispose of file parser object
    delete m_file;
    m_file = 0;
}

void IntelHexSourceFile::close()
{
    assert(m_image);

    SourceFile::close();

    // dispose of memory image
    delete m_image;
    m_image = 0;
}

//! \pre The file must be open before this method can be called.
//!
DataSource *IntelHexSourceFile::createDataSource()
{
    assert(m_image);
    return new MemoryImageDataSource(m_image);
}

//! \retval true The file has an 03 or 05 record.
//! \retval false No entry point is available.
bool IntelHexSourceFile::hasEntryPoint()
{
    return m_hasEntryRecord;
}

//! If no entry point is available then 0 is returned instead.
//!
//! \return Entry point address.
//! \retval 0 No entry point is available.
uint32_t IntelHexSourceFile::getEntryPointAddress()
{
    if (m_hasEntryRecord)
    {
        // the address in the record is the entry point
        uint32_t address = (m_entryRecord.m_data[0] << 24) | (m_entryRecord.m_data[1] << 16) |
                           (m_entryRecord.m_data[2] << 8) | (m_entryRecord.m_data[3]);
        Log::log(Logger::kDebug2, "entry point address is 0x%08x\n", address);
        return address;
    }

    return 0;
}

//! Scans the Intel Hex of the file looking for data records(00 records). The
//! contents of these records are added to an StExecutableImage object, which
//! coalesces the individual records into contiguous regions of memory.
//!
//! Also looks for 03 and 05 records that contain the entry point. The first
//! match of one of these records is saved off into the #m_entryRecord member.
//!
//! Also looks for 02 and 04 records that contain the extended address. The
//! address in the StExecutableImage object is the combination of extended
//! address and offset in data record.
//!
//! If 01 record, the file end record is detected, the left records would not
//! be handled.
//!
//! \pre The #m_file member must be valid.
//! \pre The #m_image member variable must have been instantiated.
void IntelHexSourceFile::buildMemoryImage()
{
    assert(m_file);
    assert(m_image);

    // Clear the entry point.
    m_hasEntryRecord = false;
    memset(&m_entryRecord, 0, sizeof(m_entryRecord));

    // Allocate buffer to hold data before adding it to the executable image.
    // Contiguous records are added to this buffer. When overflowed or when a
    // non-contiguous record is encountered the buffer is added to the executable
    // image where it will be coalesced further. We don't add records individually
    // to the image because coalescing record by record is very slow.
    smart_array_ptr<uint8_t> buffer = new uint8_t[COLLECTION_BUFFER_SIZE];

    unsigned baseAddress = 0;
    unsigned startAddress = 0;
    unsigned nextAddress = 0;
    unsigned dataLength = 0;

    // process IntelHexs
    StIntelHexFile::const_iterator it = m_file->getBegin();
    for (; it != m_file->getEnd(); it++)
    {
        const StIntelHexFile::IntelHex &theRecord = *it;

        bool isDataRecord = theRecord.m_type == INTELHEX_RECORD_DATA;
        bool isEntryRecord = (theRecord.m_type == INTELHEX_RECORD_START_SEGMENT_ADDRESS) ||
                             (theRecord.m_type == INTELHEX_RECORD_START_LINEAR_ADDRESS);
        bool isAddressRecord = (theRecord.m_type == INTELHEX_RECORD_EXTENDED_SEGMENT_ADDRESS) ||
                               (theRecord.m_type == INTELHEX_RECORD_EXTENDED_LINEAR_ADDRESS);
        bool isFileEndRecord = theRecord.m_type == INTELHEX_RECORD_END_OF_FILE;
        // handle 00 data record
        if (isDataRecord)
        {
            // If this record's data would overflow the collection buffer, or if the
            // record is not contiguous with the rest of the data in the collection
            // buffer, then flush the buffer to the executable image and restart.
            if (dataLength &&
                ((dataLength + theRecord.m_dataCount > COLLECTION_BUFFER_SIZE) || (theRecord.m_address != nextAddress)))
            {
                m_image->addTextRegion(startAddress + baseAddress, buffer, dataLength);

                dataLength = 0;
            }

            // Capture addresses when starting an empty buffer.
            if (dataLength == 0)
            {
                startAddress = theRecord.m_address;
                nextAddress = startAddress;
            }

            // Copy record data into place in the collection buffer and update
            // size and address.
            memcpy(&buffer[dataLength], theRecord.m_data, theRecord.m_dataCount);
            dataLength += theRecord.m_dataCount;
            nextAddress += theRecord.m_dataCount;
        }
        // handle 02, 04 record
        else if (isAddressRecord)
        {
            // If there are data in the collection buffer, then flush the buffer to the executable image.
            if (dataLength != 0)
            {
                m_image->addTextRegion(startAddress + baseAddress, buffer, dataLength);
                dataLength = 0;
            }

            // extended address stored at data field.
            baseAddress = (theRecord.m_data[0] << 8) | theRecord.m_data[1];
            if (theRecord.m_type == INTELHEX_RECORD_EXTENDED_SEGMENT_ADDRESS)
            {
                baseAddress <<= 4; // Extended Segment Address Record, left shift 4 bits.
            }
            else
            {
                baseAddress <<= 16; // Extended Linear Address Record, left shitf 16 bits.
            }
        }
        // handle 03, 05 record
        else if (isEntryRecord)
        {
            if (!m_hasEntryRecord)
            {
                // save off the entry point record so we don't have to scan again
                memcpy(&m_entryRecord, &theRecord, sizeof(m_entryRecord));
                m_hasEntryRecord = true;
            }
            else
            {
                // throw an exception when detecting a second entry record
                throw StIntelHexParseException("multiple entry record detected");
            }
        }
        // handle 01 record
        else if (isFileEndRecord)
        {
            // if the file end record is detected, stop handling the records.
            break;
        }
    }

    // Add any leftover data in the collection buffer to the executable image.
    if (dataLength)
    {
        m_image->addTextRegion(startAddress + baseAddress, buffer, dataLength);
    }
}
