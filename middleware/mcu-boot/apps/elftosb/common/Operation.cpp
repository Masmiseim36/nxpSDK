/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Operation.h"

using namespace elftosb;

//! The operation object takes ownership of \a source.
//!
//! Cross references between the target and source are updated.
void LoadOperation::setSource(DataSource *source)
{
    m_source = source;

    if (m_target)
    {
        m_target->setSource(m_source);
    }
    if (m_source)
    {
        m_source->setTarget(m_target);
    }
}

//! The operation object takes ownership of \a target.
//!
//! Cross references between the target and source are updated.
void LoadOperation::setTarget(DataTarget *target)
{
    m_target = target;

    if (m_target)
    {
        m_target->setSource(m_source);
    }
    if (m_source)
    {
        m_source->setTarget(m_target);
    }
}

void FlashEraseOperation::getRange(uint32_t *start, uint32_t *count) const
{
    if (start)
    {
        *start = m_startAddress;
    }
    if (count)
    {
        *count = m_byteCount;
    }
}

void MemEnableOperation::getRange(uint32_t *start, uint32_t *count) const
{
    if (start)
    {
        *start = m_startAddress;
    }
    if (count)
    {
        *count = m_byteCount;
    }
}

//! Disposes of operations objects in the sequence.
OperationSequence::~OperationSequence()
{
    //	iterator_t it = begin();
    //	for (; it != end(); ++it)
    //	{
    //		delete it->second;
    //	}
}

void OperationSequence::append(const OperationSequence *other)
{
    const_iterator_t it = other->begin();
    for (; it != other->end(); ++it)
    {
        m_operations.push_back(*it);
    }
}
