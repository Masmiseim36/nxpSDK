/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h> // MFC core and standard components
#include <afxext.h> // MFC extensions

#include <afxdisp.h> // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h> // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h> // MFC support for Windows Common Controls
#endif              // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h> // MFC support for ribbons and control bars

// This macro is the same as IMPLEMENT_OLECREATE, except it passes TRUE
// for the bMultiInstance parameter to the COleObjectFactory constructor.
// We want a separate instance of this application to be launched for
// each automation proxy object requested by automation controllers.
#ifndef IMPLEMENT_OLECREATE2
#define IMPLEMENT_OLECREATE2(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)       \
    AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid, RUNTIME_CLASS(class_name), TRUE, \
                                                      _T(external_name));                                \
    const AFX_DATADEF GUID class_name::guid = { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } };
#endif // IMPLEMENT_OLECREATE2

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment( \
    linker,      \
    "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment( \
    linker,      \
    "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment( \
    linker,      \
    "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define USER_MS_UPDATA_LOG WM_USER + 100
#define USER_MS_CONNECTION_CHANGE WM_USER + 101
#define USER_MS_UPDATA_STATUS WM_USER + 102
#define USER_MS_UPDATA_PROGRESS WM_USER + 103

#include <InitGuid.h>
// GUID interface for COM port device (86E0D1E0-8089-11D0-9CE4-08003E301F73)
DEFINE_GUID(GUID_DEVINTERFACE_COMPORT, 0x86E0D1E0, 0x8089, 0x11D0, 0x9C, 0xE4, 0x08, 0x00, 0x3E, 0x30, 0x1F, 0x73);
// GUID interface for USB device (A5DCBF10-6530-11D2-901F-00C04FB951ED)
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);
// GUID device class for ports (4d36E978-E325-11CE-BFC1-08002BE10318)
DEFINE_GUID(GUID_DEVCLASS_PORTS, 0x4d36E978, 0xE325, 0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);
// GUID device class for USB (36FC9E60-C465-11CF-8056-444553540000)
DEFINE_GUID(GUID_DEVCLASS_USB, 0x36FC9E60, 0xC465, 0x11CF, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);