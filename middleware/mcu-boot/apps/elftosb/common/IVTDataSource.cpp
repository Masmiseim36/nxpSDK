/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "IVTDataSource.h"
#include "DataTarget.h"
#include "EndianUtilities.h"
#include <algorithm>
#include <stdlib.h>
#include <string.h>

using namespace elftosb;

IVTDataSource::IVTDataSource()
    : DataSource()
    , DataSource::Segment((DataSource &)*this)
    , m_isSelfSet(false)
{
    // Init the IVT structure.
    memset(&m_ivt, 0, sizeof(m_ivt));
    hab_hdr_t hdr = IVT_HDR(sizeof(m_ivt), HAB_VERSION);
    m_ivt.hdr = hdr;
}

unsigned IVTDataSource::getData(unsigned offset, unsigned maxBytes, uint8_t *buffer)
{
    // Bail if the offset is out of range.
    if (offset >= sizeof(m_ivt))
    {
        return 0;
    }

    // If we have an associated target, and the IVT self pointer is not set, then
    // fill in the self pointer from the target address.
    if (m_target && !m_isSelfSet)
    {
        m_ivt.self = ENDIAN_HOST_TO_LITTLE_U32(m_target->getBeginAddress());
    }

    // Truncate max bytes at the end of the IVT.
    maxBytes = std::min<unsigned>(maxBytes, sizeof(m_ivt) - offset);

    // Copy into output buffer.
    if (maxBytes)
    {
        memcpy(buffer, (uint8_t *)&m_ivt + offset, maxBytes);
    }

    return maxBytes;
}

unsigned IVTDataSource::getLength()
{
    return sizeof(m_ivt);
}

//! The IVT has a natural location if its self pointer was explicitly specified.
//!
bool IVTDataSource::hasNaturalLocation()
{
    return m_isSelfSet;
}

//!
uint32_t IVTDataSource::getBaseAddress()
{
    return m_ivt.self;
}

bool IVTDataSource::setFieldByName(const std::string &name, uint32_t value)
{
    if (name == "entry")
    {
        m_ivt.entry = ENDIAN_HOST_TO_LITTLE_U32(value);
    }
    else if (name == "dcd")
    {
        m_ivt.dcd = ENDIAN_HOST_TO_LITTLE_U32(value);
    }
    else if (name == "boot_data")
    {
        m_ivt.boot_data = ENDIAN_HOST_TO_LITTLE_U32(value);
    }
    else if (name == "self")
    {
        m_ivt.self = ENDIAN_HOST_TO_LITTLE_U32(value);
        m_isSelfSet = true;
    }
    else if (name == "csf")
    {
        m_ivt.csf = ENDIAN_HOST_TO_LITTLE_U32(value);
    }
    else
    {
        // Unrecognized field name.
        return false;
    }

    return true;
}

BootDataSource::BootDataSource(uint32_t baseAddress)
	: DataSource()
	, DataSource::Segment((DataSource &)*this)
    , m_baseAddress(baseAddress)
{
	// Init the IVT structure.
	memset(&m_bootData, 0, sizeof(m_bootData));
}

unsigned BootDataSource::getData(unsigned offset, unsigned maxBytes, uint8_t *buffer)
{
	// Bail if the offset is out of range.
	if (offset >= sizeof(m_bootData))
	{
		return 0;
	}

	// Truncate max bytes at the end of the IVT.
	maxBytes = std::min<unsigned>(maxBytes, sizeof(m_bootData) - offset);

	// Copy into output buffer.
	if (maxBytes)
	{
		memcpy(buffer, (uint8_t *)&m_bootData + offset, maxBytes);
	}

	return maxBytes;
}

unsigned BootDataSource::getLength()
{
	return sizeof(m_bootData);
}

//! The IVT has a natural location if its self pointer was explicitly specified.
//!
bool BootDataSource::hasNaturalLocation()
{
	return false;
}

//!
uint32_t BootDataSource::getBaseAddress()
{
	return m_baseAddress;
}

bool BootDataSource::setFieldByName(const std::string &name, uint32_t value)
{
	if (name == "start")
	{
		m_bootData.startAddress = ENDIAN_HOST_TO_LITTLE_U32(value);
	}
	else if (name == "size")
	{
		m_bootData.size = ENDIAN_HOST_TO_LITTLE_U32(value);
	}
	else if (name == "flags")
	{
		m_bootData.flags = ENDIAN_HOST_TO_LITTLE_U32(value);
	}
	else
	{
		// Unrecognized field name.
		return false;
	}

	return true;
}


PaddingDataSource::PaddingDataSource(uint32_t baseAddress, uint32_t paddingSize, uint8_t paddingByte)
	: DataSource()
	, DataSource::Segment((DataSource &)*this)
	, m_baseAddress(baseAddress)
	, m_paddingSize(paddingSize)
{
	// Init the padding data.
	m_pPaddingBytes = new uint8_t[paddingSize];
	memset(m_pPaddingBytes, paddingByte, paddingSize);
}

unsigned PaddingDataSource::getData(unsigned offset, unsigned maxBytes, uint8_t *buffer)
{
	// Bail if the offset is out of range.
	if (offset >= m_paddingSize)
	{
		return 0;
	}

	// Truncate max bytes at the end of the IVT.
	maxBytes = std::min<unsigned>(maxBytes, m_paddingSize - offset);

	// Copy into output buffer.
	if (maxBytes)
	{
		memcpy(buffer, (uint8_t *)m_pPaddingBytes + offset, maxBytes);
	}

	return maxBytes;
}

unsigned PaddingDataSource::getLength()
{
	return m_paddingSize;
}

//! The IVT has a natural location if its self pointer was explicitly specified.
//!
bool PaddingDataSource::hasNaturalLocation()
{
	return false;
}

//!
uint32_t PaddingDataSource::getBaseAddress()
{
	return m_baseAddress;
}