/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blfwk/stdafx.h"
#include "blfwk/StIntelHexFile.h"
#ifdef LINUX
#include <string.h>
#endif

StIntelHexFile::StIntelHexFile(std::istream &inStream)
    : m_stream(inStream)
{
}

//! Frees any data allocated as part of an Intel Hex.
StIntelHexFile::~StIntelHexFile()
{
    const_iterator it;
    for (it = m_records.begin(); it != m_records.end(); it++)
    {
        IntelHex &theRecord = (IntelHex &)*it;
        if (theRecord.m_data)
        {
            delete[] theRecord.m_data;
            theRecord.m_data = NULL;
        }
    }
}

//! Just looks for ":" as the first characters of the file followed by two digit characters.
bool StIntelHexFile::isIntelHexFile()
{
    int savePosition = (int)m_stream.tellg();
    m_stream.seekg(0, std::ios_base::beg);

    char buffer[3];
    m_stream.read(buffer, 3);
    bool isIntelHex = (buffer[0] == ':' && isHexDigit(buffer[1]) && isHexDigit(buffer[2]));

    m_stream.seekg(savePosition, std::ios_base::beg);

    return isIntelHex;
}

//! Extract records one line at a time and hand them to the parseLine()
//! method. Either CR, LF, or CRLF line endings are supported. The input
//! stream is read until EOF or an file end record detected.
//! The parse() method must be called after the object has been constructed
//! before any of the records will become accessible.
//! \exception StIntelHexParseException will be thrown if any error occurs while
//!     parsing the input.
void StIntelHexFile::parse()
{
    // back to start of stream
    m_stream.seekg(0, std::ios_base::beg);

    std::string thisLine;

    do
    {
        char thisChar;
        m_stream.get(thisChar);

        if (thisChar == '\r' || thisChar == '\n')
        {
            // skip the LF in a CRLF
            if (thisChar == '\r' && m_stream.peek() == '\n')
                m_stream.ignore();

            // parse line if it's not empty
            if (!thisLine.empty())
            {
                parseLine(thisLine);

                // reset line
                thisLine.clear();
            }
        }
        else
        {
            thisLine += thisChar;
        }
    } while (!m_stream.eof());
}

bool StIntelHexFile::isHexDigit(char c)
{
    return (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int StIntelHexFile::hexDigitToInt(char digit)
{
    if (isdigit(digit))
        return digit - '0';
    else if (digit >= 'a' && digit <= 'f')
        return 10 + digit - 'a';
    else if (digit >= 'A' && digit <= 'F')
        return 10 + digit - 'A';

    // unknow char
    return 0;
}

//! \exception StIntelHexParseException is thrown if either of the nibble characters
//!     is not a valid hex digit.
int StIntelHexFile::readHexByte(std::string &inString, int inIndex)
{
    char nibbleCharHi = inString[inIndex];
    char nibbleCharLo = inString[inIndex + 1];

    // must be hex digits
    if (!(isHexDigit(nibbleCharHi) && isHexDigit(nibbleCharLo)))
    {
        throw StIntelHexParseException("invalid hex digit");
    }

    return (hexDigitToInt(nibbleCharHi) << 4) | hexDigitToInt(nibbleCharLo);
}

//! \brief Parses individual Intel Hex.
//!
//! Takes a single Intel Hex line as input and appends a new IntelHex struct
//! to the m_records vector.
//! \exception StIntelHexParseException will be thrown if any error occurs while
//!     parsing \a inLine.
void StIntelHexFile::parseLine(std::string &inLine)
{
    int checksum = 0;
    IntelHex newRecord;
    memset(&newRecord, 0, sizeof(newRecord));

    // must be at least a certain length
    if (inLine.length() < INTELHEX_MIN_LENGTH)
    {
        throw StIntelHexParseException("invalid record length");
    }

    // start char must be ':'
    if (inLine[0] != INTELHEX_START_CHAR)
    {
        throw StIntelHexParseException("invalid record start char");
    }

    // parse count field
    newRecord.m_dataCount = readHexByte(inLine, 1);
    checksum += newRecord.m_dataCount;

    // verify the record length now that we know the count
    if (inLine.length() != 11 + newRecord.m_dataCount * 2)
    {
        throw StIntelHexParseException("invalid record length");
    }

    // read address field.
    int address = 0;
    for (int i = 0; i < 2; ++i)
    {
        int addressByte = readHexByte(inLine, INTELHEX_ADDRESS_START_CHAR_INDEX + i * 2);
        address = (address << 8) | addressByte;
        checksum += addressByte;
    }
    newRecord.m_address = address;

    // handle data type.;
    newRecord.m_type = readHexByte(inLine, INTELHEX_TYPE_START_CHAR_INDEX);
    checksum += newRecord.m_type;
    switch (newRecord.m_type)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            break;
        default:
            throw StIntelHexParseException("unknown Intel Hex record type");
            break;
    }

    // read data
    if (newRecord.m_dataCount)
    {
        uint8_t *data = new uint8_t[newRecord.m_dataCount];
        for (unsigned i = 0; i < newRecord.m_dataCount; ++i)
        {
            int dataByte = readHexByte(inLine, INTELHEX_DATA_START_CHAR_INDEX + i * 2);
            data[i] = dataByte;
            checksum += dataByte;
        }

        newRecord.m_data = data;
    }

    // read and compare checksum byte
    checksum = (~checksum + 1) & 0xff; // low byte of one's complement of sum of other bytes
    newRecord.m_checksum = readHexByte(inLine, (int)inLine.length() - 2);
    if (checksum != newRecord.m_checksum)
    {
        throw StIntelHexParseException("invalid checksum");
    }

    // now save the new Intel Hex record
    m_records.push_back(newRecord);
}
