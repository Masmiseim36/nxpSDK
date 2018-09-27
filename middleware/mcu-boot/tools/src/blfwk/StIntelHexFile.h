/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_StIntelHexFile_h_)
#define _StIntelHexFile_h_

#include "stdafx.h"
#include <istream>
#include <string>
#include <vector>
#include <stdexcept>

enum
{
    //! The required first character of a Intel Hex
    INTELHEX_START_CHAR = ':',

    //! The minimum length of a Hex. This is the start char (1) + datacount (2) +
    //!                                             addr (4) + type (2) + check sum (2).
    INTELHEX_MIN_LENGTH = 11,

    //! Index of the first character of the address field.
    INTELHEX_ADDRESS_START_CHAR_INDEX = 3,

    //! Index of the first character of the record type field.
    INTELHEX_TYPE_START_CHAR_INDEX = 7,

    //! Index of the first character of the record type field.
    INTELHEX_DATA_START_CHAR_INDEX = 9
};

//! Intel Hex Record Type
enum
{
    //! Data Record, which contains data and a 16-bit start address for the data.
    INTELHEX_RECORD_DATA = 0x00,

    //! End of File Record, which specifies the end of the hex file, and
    //! must occur exactly once per file in the last line of the file.
    INTELHEX_RECORD_END_OF_FILE = 0x01,

    //! Extended Segment Address Record, which is used to specify bits 4- 19 of the Segment Base Address.
    INTELHEX_RECORD_EXTENDED_SEGMENT_ADDRESS = 0x02,

    //! Start Segment Address Record, which is used to specify the execution start address for the object file.
    INTELHEX_RECORD_START_SEGMENT_ADDRESS = 0x03,

    //! Extended Linear Address Record, which is used to specify bits 16- 31 of the Linear Base Address.
    INTELHEX_RECORD_EXTENDED_LINEAR_ADDRESS = 0x04,

    //! Start Linear Address Record, which is used to specify the execution start address for the object file.
    INTELHEX_RECORD_START_LINEAR_ADDRESS = 0x05
};

/*!
 * \brief Intel Hex parser.
 *
 * This class takes an input stream and parses it as a Intel Hex file. While
 * the individual records that comprise the file are available for access, the
 * class also provides a higher-level view of the contents. It processes the
 * individual records and builds an image of what the memory touched by the
 * file looks like. Then you can access the contiguous sections of memory.
 */
class StIntelHexFile
{
public:
    /*!
     * Structure representing each individual line of the Intel Hex input data.
     */
    struct IntelHex
    {
        unsigned m_dataCount; //!< The number of bytes in the data field.
        uint32_t m_address;   //!< The address offset of the data.
        unsigned m_type;      //!< Type of the data field. 00: Data
                              //!<                         01: End of File
                              //!<                         02: Extended Segment Address
                              //!<                         03: Start Segment Address
                              //!<                         04: Extended Linear Address
                              //!<                         05: Start Linear Address
        uint8_t *m_data;      //!< Pointer to data, or NULL if no data for this record.
        uint8_t m_checksum;   //!< The checksum byte used to verify the record.
    };

    //! Iterator type.
    typedef std::vector<IntelHex>::const_iterator const_iterator;

public:
    //! \brief Constructor.
    StIntelHexFile(std::istream &inStream);

    //! \brief Destructor.
    virtual ~StIntelHexFile();

    //! \name File name
    //@{
    virtual void setName(const std::string &inName) { m_name = inName; }
    virtual std::string getName() const { return m_name; }
    //@}

    //! \name Parsing
    //@{
    //! \brief Determine if the file is a Intel Hex file.
    virtual bool isIntelHexFile();

    //! \brief Parse the entire IntelHex input stream.
    virtual void parse();
    //@}

    //! \name Record access
    //@{
    //! \return the number of Intel Hex that have been parsed from the input stream.
    inline unsigned getRecordCount() const { return static_cast<unsigned>(m_records.size()); }
    //! \return iterator for
    inline const_iterator getBegin() const { return m_records.begin(); }
    inline const_iterator getEnd() const { return m_records.end(); }
    //@}

    //! \name Operators
    //@{
    inline const IntelHex &operator[](unsigned inIndex) { return m_records[inIndex]; }
    //@}

protected:
    std::istream &m_stream;          //!< The input stream for the Intel Hex data.
    std::vector<IntelHex> m_records; //!< Vector of Intel Hex in the input data.

    std::string m_name; //!< File name. (optional)

    //! \name Parsing utilities
    //@{
    virtual void parseLine(std::string &inLine);

    bool isHexDigit(char c);
    int hexDigitToInt(char digit);
    int readHexByte(std::string &inString, int inIndex);
    //@}
};

/*!
 * \brief Simple exception thrown to indicate an error in the input Intel Hex data format.
 */
class StIntelHexParseException : public std::runtime_error
{
public:
    //! \brief Default constructor.
    StIntelHexParseException(const std::string &inMessage)
        : std::runtime_error(inMessage)
    {
    }
};

#endif // _StIntelHexFile_h_
