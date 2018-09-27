/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(_Crc_h_)
#define _Crc_h_

#include <stdint.h>

namespace blfwk
{
/*!
* \brief Crc class contains the functionality about crc ccalculation.
*/
class Crc
{
public:
    //! \brief Default constructor.
    Crc(){};

    //! \brief Destructor.
    virtual ~Crc(){};

    //! \brief calculate crc32 for a given memory region
    //!
    //! \param start    The start address of a memory region that contains the data
    //! \
//! \param length   Length in bytes to calculate
    //! \
//! \return calculated result
    static uint32_t calculate_application_crc32(const uint8_t *start, uint32_t length);
    //@}
    //
};

} // namespace blfwk
#endif // _Crc_h_
