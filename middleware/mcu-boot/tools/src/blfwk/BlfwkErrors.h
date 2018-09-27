/*
 * File:    BlfwkErrors.h
 *
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_BlfwkErrors_h_)
#define _BlfwkErrors_h_

#include <string>
#include <stdexcept>

//! @addtogroup host_error
//! @{

namespace blfwk
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

}; // namespace blfwk

//! @}

#endif // _BlfwkErrors_h_
