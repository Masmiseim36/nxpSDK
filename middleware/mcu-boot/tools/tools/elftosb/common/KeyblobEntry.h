/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_KeyblobEntry_h_)
#define _KeyblobEntry_h_

#include "smart_ptr.h"
#include "OptionContext.h"

namespace elftosb
{
/*!
* @brief Base class for data model of sections of the output file.
*/
class KeyblobEntry
{
public:
    KeyblobEntry()
        : m_id(0)
        , m_options(0)
    {
    }
    KeyblobEntry(uint32_t identifier)
        : m_id(identifier)
        , m_options(0)
    {
    }
    virtual ~KeyblobEntry() {}
    void setIdentifier(uint32_t identifier) { m_id = identifier; }
    uint32_t getIdentifier() const { return m_id; }
    //! \brief Set the option context.
    //!
    //! The keyblob entry object will assume ownership of the option context
    //! and delete it when the section is deleted.
    inline void setOptions(OptionContext *context) { m_options = context; }
    //! \brief Return the option context.
    inline const OptionContext *getOptions() const { return m_options; }
protected:
    uint32_t m_id;                      //!< Unique identifier.
    smart_ptr<OptionContext> m_options; //!< Options associated with just this keyblob entry.
};

}; // namespace elftosb

#endif // _KeyblobEntry_h_
