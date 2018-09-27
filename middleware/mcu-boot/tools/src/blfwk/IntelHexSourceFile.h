/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_IntelHexSourceFile_h_)
#define _IntelHexSourceFile_h_

#include "SourceFile.h"
#include "StIntelHexFile.h"
#include "StExecutableImage.h"

namespace blfwk
{
/*!
 * \brief Executable file in the Intel Hex format.
 *
 * Instead of presenting each Intel Hex in the file separately, this class
 * builds up a memory image of all of the records. Records next to each other
 * in memory are coalesced into a single memory region. The data source that
 * is returned from createDataSource() exposes these regions as its segments.
 */
class IntelHexSourceFile : public SourceFile
{
public:
    //! \brief Default constructor.
    IntelHexSourceFile(const std::string &path);

    //! \brief Destructor.
    virtual ~IntelHexSourceFile() {}
    //! \brief Test whether the \a stream contains a valid Intel Hex file.
    static bool isIntelHexFile(std::istream &stream);

    //! \name Opening and closing
    //@{
    //! \brief Opens the file.
    virtual void open();

    //! \brief Closes the file.
    virtual void close();
    //@}

    //! \name Format capabilities
    //@{
    virtual bool supportsNamedSections() const { return false; }
    virtual bool supportsNamedSymbols() const { return false; }
    //@}

    //! \name Data sources
    //@{
    //! \brief Returns data source for the entire file.
    virtual DataSource *createDataSource();
    //@}

    //! \name Entry point
    //@{
    //! \brief Returns true if an entry point was set in the file.
    virtual bool hasEntryPoint();

    //! \brief Returns the entry point address.
    virtual uint32_t getEntryPointAddress();
    //@}

protected:
    StIntelHexFile *m_file;                 //!< Intel Hex parser instance.
    StExecutableImage *m_image;             //!< Memory image of the Intel Hex file..
    bool m_hasEntryRecord;                  //!< Whether a type 03 or 05 record was found.
    StIntelHexFile::IntelHex m_entryRecord; //!< Record for the entry point.

protected:
    //! \brief Build memory image of the Intel Hex file.
    void buildMemoryImage();
};

}; // namespace blfwk

#endif // _IntelHexSourceFile_h_
