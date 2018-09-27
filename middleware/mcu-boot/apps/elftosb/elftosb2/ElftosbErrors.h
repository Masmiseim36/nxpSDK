/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_ElftosbErrors_h_)
#define _ElftosbErrors_h_

#include <string>
#include <stdexcept>

namespace elftosb
{
/*!
 * \brief A semantic error discovered while processing the command file AST.
 */
class semantic_error : public std::runtime_error
{
public:
    explicit semantic_error(const std::string &msg)
        : std::runtime_error(msg)
    {
    }
};

}; // namespace elftosb

#endif // _ElftosbErrors_h_
