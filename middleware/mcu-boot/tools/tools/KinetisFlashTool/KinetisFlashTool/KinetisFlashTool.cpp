/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

// KinetisFlashTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KinetisFlashTool.h"
#include "KinetisFlashToolDlg.h"
#include "SerialList.h"
#include "HistoryData.h"
#include "Bootloader.h"
#include "utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment(lib, "../Debug/blfwkdll.lib")
#else
#pragma comment(lib, "../Release/blfwkdll.lib")
#endif

#define DEFAULT_INI_FILE_NAME _T("KinetisFlashTool.ini")
#define DEFAULT_BLHOST_PATH _T("blhost.exe")
#define PROFILE_SEPARATOR _T(";")
#define MAX_PROFILE_STRING_LENGTH (MAX_PATH * 11)

extern CKinetisFlashToolDlg *g_pMainDlg;

// CKinetisFlashToolApp

BEGIN_MESSAGE_MAP(CKinetisFlashToolApp, CWinApp)
ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CKinetisFlashToolApp construction

CKinetisFlashToolApp::CKinetisFlashToolApp()
{
    // support Restart Manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only CKinetisFlashToolApp object

CKinetisFlashToolApp theApp;

const GUID CDECL BASED_CODE _tlid = { 0x25459355, 0x51EB, 0x4354, { 0xA5, 0x52, 0x6, 0x97, 0xFE, 0x73, 0x43, 0xF5 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

// CKinetisFlashToolApp initialization

BOOL CKinetisFlashToolApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    CWinApp::InitInstance();

    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }
    AfxEnableControlContainer();

    g_pAllComDevices = new SerialList();
    g_pAllComDevices->ScanSerialDevices();
    g_pHistoryData = new HistoryData();

    ParseIniFile(DEFAULT_INI_FILE_NAME);

    g_pBootloader = new Bootloader();
    // g_pBootloader->Open();

    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager *pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    // Parse command line for automation or reg/unreg switches.
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // App was launched with /Embedding or /Automation switch.
    // Run app as automation server.
    if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
    {
        // Register class factories via CoRegisterClassObject().
        COleTemplateServer::RegisterAll();
    }
    // App was launched with /Unregserver or /Unregister switch.  Remove
    // entries from the registry.
    else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
    {
        COleObjectFactory::UpdateRegistryAll(FALSE);
        AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
        return FALSE;
    }
    // App was launched standalone or with other switches (e.g. /Register
    // or /Regserver).  Update registry entries, including typelibrary.
    else
    {
        COleObjectFactory::UpdateRegistryAll();
        AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
        if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
            return FALSE;
    }

    CKinetisFlashToolDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
        TRACE(traceAppMsg, 0,
              "Warning: if you are using MFC controls on the dialog, you cannot #define "
              "_AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    }

    // Delete the shell manager created above.
    if (pShellManager != NULL)
    {
        delete pShellManager;
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

int CKinetisFlashToolApp::ExitInstance()
{
    AfxOleTerm(FALSE);

    WriteIniFile(DEFAULT_INI_FILE_NAME);
    delete g_pAllComDevices;
    delete g_pHistoryData;
    // g_pBootloader->Close();
    delete g_pBootloader;
    return CWinApp::ExitInstance();
}

// See CKinetisFlashToolApp.h for documentation of this method.
bool CKinetisFlashToolApp::ParseIniFile(CString strFilename)
{
    // Get EXE path
    CString strFilePath;
    GetModuleFileName(NULL, strFilePath.GetBuffer(_MAX_PATH), _MAX_PATH);
    strFilePath.ReleaseBuffer();
    int pos = strFilePath.ReverseFind(_T('\\'));
    strFilePath = strFilePath.Left(pos + 1); //+1 for add '\' at the last
    strFilePath.Append(strFilename);         // add .ini file name to the path.

    TCHAR cBuffer[MAX_PROFILE_STRING_LENGTH];
    if (g_pAllComDevices->GetDeviceCount() > 0)
    {
        g_pHistoryData->SetCurrentDevice(0);
        if (GetPrivateProfileString(_T("Port"), _T("Current"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH,
                                    strFilePath) > 0)
        {
            for (size_t i = 0; i < g_pAllComDevices->GetDeviceCount(); i++)
            {
                if (g_pAllComDevices->GetDevice(i).GetString().Compare(cBuffer) == 0)
                {
                    g_pHistoryData->SetCurrentDevice(i);
                    break;
                }
            }
        }
    }
    else
    {
        g_pHistoryData->SetCurrentDevice(-1); // Invalid index
    }

    if (GetPrivateProfileString(_T("Baudrate"), _T("List"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH, strFilePath) >
        0)
    {
        CString initBaudList = CString(cBuffer);
        int32_t baud;
        for (size_t i = 0; i < 10; i++)
        {
            pos = initBaudList.Find(PROFILE_SEPARATOR);
            if (pos >= 0)
            {
                if (pos != 0)
                {
                    if (cstringtoi(initBaudList.Left(pos), baud))
                    {
                        g_pHistoryData->AddBaudRate(baud);
                    }
                }
                initBaudList = initBaudList.Right(initBaudList.GetLength() - pos - 1);
            }
            else // pos == -1
            {
                if (!initBaudList.IsEmpty())
                {
                    if (cstringtoi(initBaudList, baud))
                    {
                        g_pHistoryData->AddBaudRate(baud);
                    }
                }
                break;
            }
        }
    }
    else
    {
        // If there is not ini file or read the ini file failed, set to default baudrate list.
        g_pHistoryData->AddBaudRate(4800);
        g_pHistoryData->AddBaudRate(9600);
        g_pHistoryData->AddBaudRate(19200);
        g_pHistoryData->AddBaudRate(38400);
        g_pHistoryData->AddBaudRate(56000);
        g_pHistoryData->AddBaudRate(57600);
        g_pHistoryData->AddBaudRate(115200);
        g_pHistoryData->AddBaudRate(128000);
        g_pHistoryData->AddBaudRate(256000);
    }

    g_pHistoryData->SetCurrentBaudRate(0);
    int32_t baud;
    if (GetPrivateProfileString(_T("Baudrate"), _T("Current"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH,
                                strFilePath) > 0)
    {
        if (!cstringtoi(cBuffer, baud))
        {
            baud = 57600;
        }
    }
    else
    {
        baud = 57600;
    }
    for (size_t i = 0; i < g_pHistoryData->GetBaudRateCount(); i++)
    {
        if (g_pHistoryData->GetBaudRate(i) == baud)
        {
            g_pHistoryData->SetCurrentBaudRate(i);
            break;
        }
    }

    if (GetPrivateProfileString(_T("VID"), _T("List"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH, strFilePath) > 0)
    {
        CString initVidList = CString(cBuffer);
        uint32_t vid;
        for (size_t i = 0; i < 10; i++)
        {
            pos = initVidList.Find(PROFILE_SEPARATOR);
            if (pos >= 0)
            {
                if (pos != 0)
                {
                    if (cstringtoui(initVidList.Left(pos), vid))
                    {
                        g_pHistoryData->AddVid(static_cast<uint16_t>(vid));
                    }
                }
                initVidList = initVidList.Right(initVidList.GetLength() - pos - 1);
            }
            else // pos == -1
            {
                if (!initVidList.IsEmpty())
                {
                    if (cstringtoui(initVidList, vid))
                    {
                        g_pHistoryData->AddVid(static_cast<uint16_t>(vid));
                    }
                }
                break;
            }
        }
    }
    else
    {
        // If there is not ini file or read the ini file failed, set to default VID list.
        g_pHistoryData->AddVid(0x15A2);
    }

    if (GetPrivateProfileString(_T("PID"), _T("List"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH, strFilePath) > 0)
    {
        CString initPidList = CString(cBuffer);
        uint32_t pid;
        for (size_t i = 0; i < 10; i++)
        {
            pos = initPidList.Find(PROFILE_SEPARATOR);
            if (pos >= 0)
            {
                if (pos != 0)
                {
                    if (cstringtoui(initPidList.Left(pos), pid))
                    {
                        g_pHistoryData->AddPid(static_cast<uint16_t>(pid));
                    }
                }
                initPidList = initPidList.Right(initPidList.GetLength() - pos - 1);
            }
            else // pos == -1
            {
                if (!initPidList.IsEmpty())
                {
                    if (cstringtoui(initPidList, pid))
                    {
                        g_pHistoryData->AddPid(static_cast<uint16_t>(pid));
                    }
                }
                break;
            }
        }
    }
    else
    {
        // If there is not ini file or read the ini file failed, set to default PID list.
        g_pHistoryData->AddPid(0x0073);
    }

    if (GetPrivateProfileString(_T("File"), _T("List"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH, strFilePath) > 0)
    {
        CString initFileList = CString(cBuffer);
        for (size_t i = 0; i < 10; i++)
        {
            pos = initFileList.Find(PROFILE_SEPARATOR);
            if (pos >= 0)
            {
                if (pos != 0)
                {
                    if (file_exists(initFileList.Left(pos)))
                    {
                        g_pHistoryData->AddFile(initFileList.Left(pos));
                    }
                }
                initFileList = initFileList.Right(initFileList.GetLength() - pos - 1);
            }
            else // pos == -1
            {
                if (!initFileList.IsEmpty())
                {
                    if (file_exists(initFileList))
                    {
                        g_pHistoryData->AddFile(initFileList);
                    }
                }
                break;
            }
        }
    }

    if (GetPrivateProfileString(_T("File"), _T("Address"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH, strFilePath) > 0)
    {
        uint32_t address;
        if (cstringtoui(cBuffer, address))
        {
            g_pHistoryData->SetImageAddress(address);
        }
        else
        {
            // Illegal history data, set to default image address(0x00000000).
            g_pHistoryData->SetImageAddress(0);
        }
    }
    else
    {
        // If there is not ini file or read the ini file failed, set to default image address(0x00000000).
        g_pHistoryData->SetImageAddress(0);
    }

    if (GetPrivateProfileString(_T("File"), _T("Backdoorkey"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH,
                                strFilePath) > 0)
    {
        g_pHistoryData->SetBackdoorKey(cBuffer);
    }
    else
    {
        // If there is not ini file or read the ini file failed, set to default backdoorKey(0102030405060708).
        g_pHistoryData->SetBackdoorKey(_T("0102030405060708"));
    }

    if (GetPrivateProfileString(_T("Device"), _T("List"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH, strFilePath) > 0)
    {
        CString DeviceList = CString(cBuffer);
        for (;;) /*parse untill no separator*/
        {
            pos = DeviceList.Find(PROFILE_SEPARATOR);
            if (pos >= 0)
            {
                if (pos != 0)
                {
                    uint32_t digits;
                    CString description;
                    if (g_pHistoryData->praseMemoryString(DeviceList.Left(pos), digits, description))
                    {
                        g_pHistoryData->AddMemory(digits, description);
                    }
                }
                DeviceList = DeviceList.Right(DeviceList.GetLength() - pos - 1);
            }
            else // pos == -1
            {
                if (!DeviceList.IsEmpty())
                {
                    uint32_t digits;
                    CString description;
                    if (g_pHistoryData->praseMemoryString(DeviceList, digits, description))
                    {
                        g_pHistoryData->AddMemory(digits, description);
                    }
                }
                break;
            }
        }
    }
    else
    {
        blfwkdll::Updater::MemoryList memoryList;
        blfwkdll::Updater::getSupportedMemories(memoryList);
        for (blfwkdll::Updater::MemoryList::iterator it = memoryList.begin(); it != memoryList.end(); it++)
        {
            g_pHistoryData->AddMemory(it->first, CString(it->second.c_str()));
        }
    }

    uint32_t digits;
    if (GetPrivateProfileString(_T("Device"), _T("Current"), _T(""), cBuffer, MAX_PROFILE_STRING_LENGTH, strFilePath) >
        0)
    {
        if (!cstringtoui(cBuffer, digits))
        {
            digits = 0;
        }
    }
    else
    {
        digits = 0;
    }
    // Check if the selected memory ID is in the list.
    if (g_pHistoryData->FindMemory(digits, NULL) != -1)
    {
        // exist, then set it as selected.
        g_pHistoryData->SetCurrentMemoryId(g_pHistoryData->FindMemory(digits, NULL));
    }
    else
    {
        // If not exist, select the index 0.
        g_pHistoryData->SetCurrentMemoryId(0);
    }

    return TRUE;
}

// See CKinetisFlashToolApp.h for documentation of this method.
bool CKinetisFlashToolApp::WriteIniFile(CString strFilename)
{
    bool status = true;
    CString strFilePath;
    GetModuleFileName(NULL, strFilePath.GetBuffer(_MAX_PATH), _MAX_PATH);
    strFilePath.ReleaseBuffer();
    int pos = strFilePath.ReverseFind(_T('\\'));
    strFilePath = strFilePath.Left(pos + 1); //+1 for add '\' at the last
    strFilePath.Append(strFilename);         // add .ini file name to the path.

    // CFile testFileExist;
    // if (!(testFileExist.Open(strFilePath, CFile::modeRead, NULL)))
    //{
    //    createNewFile = true;
    //}
    // testFileExist.Close();

    CString ctrBuffer;
    if ((g_pAllComDevices->GetDeviceCount() > 0) && (g_pHistoryData->GetCurrentDevice() >= 0))
    {
        if (!WritePrivateProfileString(_T("Port"), _T("Current"),
                                       g_pAllComDevices->GetDevice(g_pHistoryData->GetCurrentDevice()).GetString(),
                                       strFilePath))
        {
            status = false;
        }
    }

    ctrBuffer.Empty();
    for (size_t i = 0; i < g_pHistoryData->GetBaudRateCount(); i++)
    {
        ctrBuffer.Append(format_cstring(_T("%d"), g_pHistoryData->GetBaudRate(i)) + PROFILE_SEPARATOR);
    }
    ctrBuffer = ctrBuffer.Left(ctrBuffer.GetLength() - 1);
    if (!WritePrivateProfileString(_T("Baudrate"), _T("List"), ctrBuffer, strFilePath))
    {
        status = false;
    }
    if (!WritePrivateProfileString(
            _T("Baudrate"), _T("Current"),
            format_cstring(_T("%d"), g_pHistoryData->GetBaudRate(g_pHistoryData->GetCurrentBaudRate())), strFilePath))
    {
        status = false;
    }

    ctrBuffer.Empty();
    for (size_t i = 0; i < g_pHistoryData->GetVidCount(); i++)
    {
        ctrBuffer.Append(format_cstring(_T("0x%04X"), g_pHistoryData->GetVid(i)) + PROFILE_SEPARATOR);
    }
    ctrBuffer = ctrBuffer.Left(ctrBuffer.GetLength() - 1);
    if (!WritePrivateProfileString(_T("VID"), _T("List"), ctrBuffer, strFilePath))
    {
        status = false;
    }

    ctrBuffer.Empty();
    for (size_t i = 0; i < g_pHistoryData->GetPidCount(); i++)
    {
        ctrBuffer.Append(format_cstring(_T("0x%04X"), g_pHistoryData->GetPid(i)) + PROFILE_SEPARATOR);
    }
    ctrBuffer = ctrBuffer.Left(ctrBuffer.GetLength() - 1);
    if (!WritePrivateProfileString(_T("PID"), _T("List"), ctrBuffer, strFilePath))
    {
        status = false;
    }

    ctrBuffer.Empty();
    for (size_t i = 0; i < g_pHistoryData->GetFileCount(); i++)
    {
        ctrBuffer.Append(g_pHistoryData->GetFilePath(i) + PROFILE_SEPARATOR);
    }
    ctrBuffer = ctrBuffer.Left(ctrBuffer.GetLength() - 1);
    if (!WritePrivateProfileString(_T("File"), _T("List"), ctrBuffer, strFilePath))
    {
        status = false;
    }
    if (!WritePrivateProfileString(_T("File"), _T("Address"),
                                   format_cstring(_T("0x%08x"), g_pHistoryData->GetImageAddress()), strFilePath))
    {
        status = false;
    }
    if (!WritePrivateProfileString(_T("File"), _T("Backdoorkey"), g_pHistoryData->GetBackdoorKey(), strFilePath))
    {
        status = false;
    }

    ctrBuffer.Empty();
    for (size_t i = 0; i < g_pHistoryData->GetMemoryCount(); i++)
    {
        ctrBuffer.Append(format_cstring(_T("0x%X"), g_pHistoryData->GetMemory(i).first) + _T(":") +
                         g_pHistoryData->GetMemory(i).second + PROFILE_SEPARATOR);
    }
    ctrBuffer = ctrBuffer.Left(ctrBuffer.GetLength() - 1);
    if (!WritePrivateProfileString(_T("Device"), _T("List"), ctrBuffer, strFilePath))
    {
        status = false;
    }
    if (!WritePrivateProfileString(
            _T("Device"), _T("Current"),
            format_cstring(_T("0x%X"), g_pHistoryData->GetMemory(g_pHistoryData->GetCurrentMemoryId()).first),
            strFilePath))
    {
        status = false;
    }
    return status;
}

//! \brief Print a log information to Log window.
void PrintLog(const wchar_t *format, ...)
{
    if (g_pMainDlg != NULL)
    {
        LPWSTR buffer;
        va_list args;
        int len;

        va_start(args, format);
        len = _vsctprintf(format, args) + 1;
        buffer = (LPWSTR)malloc(len * sizeof(WCHAR));
        std::vswprintf(buffer, len, format, args);
        va_end(args);

        ::SendMessage(*g_pMainDlg, USER_MS_UPDATA_LOG, (WPARAM)(buffer), 0);
        free(buffer);
    }
}
