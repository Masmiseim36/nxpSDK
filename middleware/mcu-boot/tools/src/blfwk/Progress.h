/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _Progress_h_
#define _Progress_h_

#pragma once

#include "blfwk/host_types.h"

/*!
* @brief Contains the callback function for progress and abort phase.
*
*/
class Progress
{
public:
    //! @brief Default constructor.
    Progress()
        : m_segmentIndex(1)
        , m_segmentCount(1)
        , m_progressCallback(NULL)
        , m_abortPhase(NULL)
    {
    }

    //! @brief Constructor with initial callback.
    Progress(void(*callback)(int, int, int), bool *abortPhase)
        : m_segmentIndex(1)
        , m_segmentCount(1)
        , m_progressCallback(callback)
        , m_abortPhase(abortPhase)
    {
    }

    //! @brief Default destructor.
    ~Progress() {}
    //! @brief execute the progress callback function.
    //!
    //! @param percentage the percentage of current executed progress.
    void progressCallback(int percentage)
    {
        if (m_progressCallback != NULL)
        {
            m_progressCallback(percentage, m_segmentIndex, m_segmentCount);
        }
    }

    //! @brief Check whether the data phase is canceled.
    bool abortPhase(void)
    {
        if ((m_abortPhase) && (*m_abortPhase == true))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //! @biref initialized the progress callback function and the variable of controlling data phase.
    //!
    //! @param callback The progress callback function.
    //!
    //! @param abortPhase The pointer pointing to a variable controlling whether abort current data phase.
    void registerCallback(void(*callback)(int, int, int), bool *abortPhase)
    {
        m_progressCallback = callback;
        m_abortPhase = abortPhase;
    }

public:
    int m_segmentIndex; //!< Index of data segment in progressing.
    int m_segmentCount; //!< The number of data segments.

private:
    void(*m_progressCallback)(int percentage, int segmentIndex, int segmentCount); //!< The progress callback function.
    bool *m_abortPhase; //!< The pointer pointing to a variable controlling whether abort current data phase.
};

#endif // _Progress_h_
