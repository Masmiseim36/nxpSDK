/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// KinetisFlashTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h" // main symbols

// CKinetisFlashToolApp:
// See KinetisFlashTool.cpp for the implementation of this class
//

class CKinetisFlashToolApp : public CWinApp
{
public:
    CKinetisFlashToolApp();

    // Overrides
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // Implementation

    DECLARE_MESSAGE_MAP()

public:
    //! \brief Parse init file to get the history information.
    bool ParseIniFile(CString strFilename);

    //! \brief Save history information to init file.
    bool WriteIniFile(CString strFilename);
};

extern CKinetisFlashToolApp theApp;