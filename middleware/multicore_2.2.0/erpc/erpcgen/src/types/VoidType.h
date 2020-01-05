/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EMBEDDED_RPC__VOIDTYPE_H_
#define _EMBEDDED_RPC__VOIDTYPE_H_

#include "DataType.h"
#include <string>

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

namespace erpcgen {

/*!
 * @brief Void data type.
 */
class VoidType : public DataType
{
public:
    /*!
     * @brief Constructor.
     *
     * This function set name to "(void)".
     */
    VoidType()
    : DataType("(void)", kVoidType)
    {
    }

    /*!
     * @brief This function return "true" value for identify void type.
     *
     * @retval true Always return true.
     */
    virtual bool isVoid() const { return true; }

    /*!
     * @brief This function returns description about the void.
     *
     * @retval "<void>" Always return "<void>" as string.
     *
     * @see std::string AliasType::getDescription() const
     * @see std::string EnumType::getDescription() const
     * @see std::string EnumMember::getDescription() const
     * @see std::string StructMember::getDescription() const
     * @see std::string StructType::getDescription() const
     * @see std::string ArrayType::getDescription() const
     * @see std::string ListType::getDescription() const
     */
    virtual std::string getDescription() const { return "<void>"; }
};

} // namespace erpcgen

#endif // _EMBEDDED_RPC__VOIDTYPE_H_
