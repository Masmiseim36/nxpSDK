/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_OptionContext_h_)
#define _OptionContext_h_

#include <string>
#include "Value.h"

namespace elftosb
{
/*!
 * \brief Pure abstract interface class to a table of options.
 */
class OptionContext
{
public:
    //! Make the dtor virtual.
    virtual ~OptionContext() {}
    //! \brief Detemine whether the named option is present in the table.
    //! \param name The name of the option to query.
    //! \retval true The option is present and has a value.
    //! \retval false No option with that name is in the table.
    virtual bool hasOption(const std::string &name) const = 0;

    //! \brief Returns the option's value.
    //! \param name The name of the option.
    //! \return The value for the option named \a name.
    //! \retval NULL No option is in the table with that name.
    virtual const Value *getOption(const std::string &name) const = 0;

    //! \brief Adds or changes an option's value.
    //!
    //! If the option was not already present in the table, it is added.
    //! Otherwise the old value is replaced.
    //!
    //! \param name The option's name.
    //! \param value New value for the option.
    virtual void setOption(const std::string &name, Value *value) = 0;

    //! \brief Removes an option from the table.
    //! \param name The name of the option to remove.
    virtual void deleteOption(const std::string &name) = 0;
};

}; // namespace elftosb

#endif // _OptionContext_h_
