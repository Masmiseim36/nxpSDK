/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include "KinetisFlashTool.h"
#include "UpdateDlg.h"
#include "afxdialogex.h"
#include "Bootloader.h"
#include "HistoryData.h"
#include "utility.h"
#include "KinetisFlashToolDlg.h"

extern void PrintLog(const wchar_t *format, ...);
extern CKinetisFlashToolDlg *g_pMainDlg;
// UpdateDlg Dialog

IMPLEMENT_DYNAMIC(UpdateDlg, CDialogEx)

// See UpdateDlg.h for documentation of this method.
UpdateDlg::UpdateDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_UPDATE, pParent)
    , m_address(_T(""))
    , m_backdoorKey(_T(""))
    , m_doErasetoUnlock(false)
    , m_useKeytoUnlock(false)
{
}

// See UpdateDlg.h for documentation of this method.
UpdateDlg::~UpdateDlg()
{
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ADDRESS, m_address);
    DDX_Control(pDX, IDC_COMBO_FILE, m_ctrlImageFile);
    DDX_Text(pDX, IDC_EDIT_KEY, m_backdoorKey);
    DDX_Control(pDX, IDC_PROGRESS_BAR, m_progressCtrl);
    DDX_Control(pDX, IDC_COMBO_MEMID, m_comboMemoryID);
}

BEGIN_MESSAGE_MAP(UpdateDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON_UPDATE, &UpdateDlg::OnBnClickedButtonUpdate)
ON_BN_CLICKED(IDC_BUTTON_BROWSE, &UpdateDlg::OnBnClickedButtonBrowse)
ON_BN_CLICKED(IDC_RADIO_ERASE, &UpdateDlg::OnBnClickedRadioErase)
ON_BN_CLICKED(IDC_RADIO_UNLOCK, &UpdateDlg::OnBnClickedRadioUnlock)
ON_EN_CHANGE(IDC_EDIT_ADDRESS, &UpdateDlg::OnChangeEdit)
ON_EN_CHANGE(IDC_EDIT_KEY, &UpdateDlg::OnChangeEdit)
ON_CBN_SELCHANGE(IDC_COMBO_FILE, &UpdateDlg::OnSelchangeComboFile)
ON_CBN_KILLFOCUS(IDC_COMBO_FILE, &UpdateDlg::OnKillfocusComboFile)
ON_WM_SHOWWINDOW()
// User defined message
ON_MESSAGE(USER_MS_UPDATA_PROGRESS, &UpdateDlg::OnUpdateProgress)
ON_CBN_SELCHANGE(IDC_COMBO_MEMID, &UpdateDlg::OnSelchangeComboMemid)
ON_CBN_KILLFOCUS(IDC_COMBO_MEMID, &UpdateDlg::OnKillfocusComboMemid)
END_MESSAGE_MAP()

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnOK()
{
    // CDialogEx::OnOK();
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnCancel()
{
    // CDialogEx::OnCancel();
}

// See UpdateDlg.h for documentation of this method.
BOOL UpdateDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO: Add extra initialization here
    // Init the image file list
    RefreshImageFileList();

    // Load saved image address.
    m_address = format_cstring(_T("0x%08X"), g_pHistoryData->GetImageAddress());
    // Max num of characters is 10, including "0x" leading
    ((CEdit *)GetDlgItem(IDC_EDIT_ADDRESS))->SetLimitText(10);

    RefreshMemoryIDList();

    // Disable check of auto connect
    // This control only takes effect when connected.
    ((CButton *)GetDlgItem(IDC_CHECK_AUTOCNT))->SetCheck(false);

    // Init values are false.
    ((CButton *)GetDlgItem(IDC_RADIO_ERASE))->SetCheck(m_doErasetoUnlock);
    ((CButton *)GetDlgItem(IDC_RADIO_UNLOCK))->SetCheck(m_useKeytoUnlock);
    GetDlgItem(IDC_STATIC_KEY)->EnableWindow(false);
    GetDlgItem(IDC_EDIT_KEY)->EnableWindow(false);

    m_progressCtrl.SetRange(0, 100);

    // Load save backdoor key
    m_backdoorKey = g_pHistoryData->GetBackdoorKey();
    // Max num of characters is 16. Must hex digits without "0x" leading.
    ((CEdit *)GetDlgItem(IDC_EDIT_KEY))->SetLimitText(16);

    UpdateData(FALSE);
    return TRUE; // return TRUE unless you set the focus to a control
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialogEx::OnShowWindow(bShow, nStatus);

    if (bShow)
    {
        // Config page might change the image file list.
        // Need refreshing every time open the update page.
        RefreshImageFileList();
        RefreshMemoryIDList();
    }
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::RefreshImageFileList()
{
    // Clear the content.
    m_ctrlImageFile.ResetContent();

    for (size_t i = 0; i < g_pHistoryData->GetFileCount(); i++)
    {
        m_ctrlImageFile.AddString(g_pHistoryData->GetFilePath(i));
    }

    if (g_pHistoryData->GetFileCount() > 0)
    {
        m_ctrlImageFile.SetCurSel(0);
        if (g_pHistoryData->GetFileType(0) == HistoryData::FileType::kBinary)
        {
            // Enable image address if a binary file is selected.
            GetDlgItem(IDC_STATIC_ADDRESS)->EnableWindow(true);
            GetDlgItem(IDC_EDIT_ADDRESS)->EnableWindow(true);
        }
        else
        {
            GetDlgItem(IDC_STATIC_ADDRESS)->EnableWindow(false);
            GetDlgItem(IDC_EDIT_ADDRESS)->EnableWindow(false);
        }
        GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(true);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(false);
    }
}

// See FlashDlg.h for documentation of this method.
void UpdateDlg::RefreshMemoryIDList()
{
    // Clear the content.
    m_comboMemoryID.ResetContent();

    for (size_t i = 0; i < g_pHistoryData->GetMemoryCount(); i++)
    {
        if (blfwkdll::Updater::IsMemorySpaceId(g_pHistoryData->GetMemory(i).first))
        {
            m_comboMemoryID.AddString(g_pHistoryData->GetMemory(i).second);
        }
    }

    if (m_comboMemoryID.SelectString(
            0, g_pHistoryData->GetMemory(g_pHistoryData->GetCurrentMemoryId()).second.GetBuffer()) == CB_ERR)
    {
        m_comboMemoryID.SetCurSel(0); // If selected memory id is not in the list, selected 0 by default.
    }
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnBnClickedButtonBrowse()
{
    CFileDialog cFileDlg(TRUE);

    cFileDlg.m_ofn.lpstrTitle = _T("Select the image file");
    cFileDlg.m_ofn.lpstrFilter =
        _T("Supported files(*.bin, *.sb, *.srec, *.s19, ")
        _T("*.hex)\0*.bin;*.sb;*.srec;*.s19;*.hex\0All files(*.*)\0*.*\0\0");
    if (IDOK == cFileDlg.DoModal())
    {
        CString fullPath = cFileDlg.GetPathName().GetBuffer();
        if (file_exists(fullPath))
        {
            g_pHistoryData->InsertFile(fullPath);
            RefreshImageFileList();
        }
        else
        {
            PrintLog(_T("Invalid full path filename, or file doesn't exist.(%s)"), fullPath);
        }
    }
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnSelchangeComboFile()
{
    g_pHistoryData->BubbleSelectedFile(m_ctrlImageFile.GetCurSel());
    RefreshImageFileList();
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnKillfocusComboFile()
{
    if (m_ctrlImageFile.GetCurSel() >= 0)
    {
        // If no change in the textbox, then return;
        return;
    }

    CString filePath;
    m_ctrlImageFile.GetWindowTextW(filePath);

    // Test if the file exists
    if (file_exists(filePath))
    {
        // File exists, add it to history file list, and refresh commbo box.
        g_pHistoryData->InsertFile(filePath);
        RefreshImageFileList();
    }
    else
    {
        // File doesn't exist.
        if (!filePath.IsEmpty())
        {
            PrintLog(_T("Invalid full path filename, or file doesn't exist.(%s)"), filePath);
        }
        m_ctrlImageFile.SetCurSel(-1); // Set invalid selection.
        GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(false);
    }
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnSelchangeComboMemid()
{
    CString memoryIdStr;
    m_comboMemoryID.GetLBText(m_comboMemoryID.GetCurSel(), memoryIdStr);
    g_pHistoryData->SetCurrentMemoryId(g_pHistoryData->FindMemory(memoryIdStr, NULL));
    RefreshMemoryIDList();
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnKillfocusComboMemid()
{
    if (m_comboMemoryID.GetCurSel() >= 0)
    {
        // If no change in the textbox, then return;
        return;
    }

    CString str;
    m_comboMemoryID.GetWindowTextW(str); // Get the input text.

    uint32_t digits;
    if (cstringtoui(str, digits))
    {
        if (g_pHistoryData->FindMemory(digits, &str) == -1) // Convert digits to memory id string.
        {
            // Invalid digits, restore to previous selection.
            PrintLog(_T("Error: Unknown Memory ID digits(%s)."), str);
            RefreshMemoryIDList();
            return;
        }
    }

    if (g_pHistoryData->FindMemoryBySubStr(str, NULL) != -1)
    {
        g_pHistoryData->SetCurrentMemoryId(g_pHistoryData->FindMemoryBySubStr(str, NULL));
    }
    else
    {
        uint32_t digits;
        CString description;
        if (g_pHistoryData->praseMemoryString(str, digits, description))
        {
            if (g_pHistoryData->AddMemory(digits, description))
            {
                PrintLog(_T("Add new memory id %#x : %s."),
                         g_pHistoryData->GetMemory(g_pHistoryData->GetMemoryCount() - 1).first,
                         g_pHistoryData->GetMemory(g_pHistoryData->GetMemoryCount() - 1).second);
                g_pHistoryData->SetCurrentMemoryId(g_pHistoryData->GetMemoryCount() - 1);
            }
            else
            {
                PrintLog(_T("Error: the Memory ID(%s) has already been used."));
            }
        }
        else
        {
            PrintLog(_T("Error: Unknown Memory ID(%s), use the format \"digits:string\" to add a new one."), str);
        }
    }
    RefreshMemoryIDList();
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnBnClickedRadioErase()
{
    // Invert the state, and update the UI
    m_doErasetoUnlock = !m_doErasetoUnlock;
    ((CButton *)GetDlgItem(IDC_RADIO_ERASE))->SetCheck(m_doErasetoUnlock);

    if (m_doErasetoUnlock)
    {
        // if check the flash-erase-all-unsecure, disable backdoor key text input.
        GetDlgItem(IDC_STATIC_KEY)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_KEY)->EnableWindow(false);

        // And uncheck flash-security-disable.
        m_useKeytoUnlock = false;
        ((CButton *)GetDlgItem(IDC_RADIO_UNLOCK))->SetCheck(false);
    }
#ifdef _DEBUG
    PrintLog(_T("Erase = %s, Unlock = %s"), m_doErasetoUnlock ? _T("True") : _T("False"),
             m_useKeytoUnlock ? _T("True") : _T("False"));
#endif
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnBnClickedRadioUnlock()
{
    // Invert the state, and update the UI
    m_useKeytoUnlock = !m_useKeytoUnlock;
    ((CButton *)GetDlgItem(IDC_RADIO_UNLOCK))->SetCheck(m_useKeytoUnlock);

    if (m_useKeytoUnlock)
    {
        // if check the flash-security-disable, enable backdoor key text input.
        GetDlgItem(IDC_STATIC_KEY)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_KEY)->EnableWindow(true);

        // And uncheck flash-erase-all-unsecure.
        m_doErasetoUnlock = false;
        ((CButton *)GetDlgItem(IDC_RADIO_ERASE))->SetCheck(false);
    }
    else
    {
        // if uncheck the flash-erase-all-unsecure, disable backdoor key text input.
        GetDlgItem(IDC_STATIC_KEY)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_KEY)->EnableWindow(false);
    }
#ifdef _DEBUG
    PrintLog(_T("Erase = %s, Unlock = %s"), m_doErasetoUnlock ? _T("True") : _T("False"),
             m_useKeytoUnlock ? _T("True") : _T("False"));
#endif
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnChangeEdit()
{
    // update UI to variables(m_address and m_backdoorKey)
    UpdateData(true);
}

// See UpdateDlg.h for documentation of this method.
void UpdateDlg::OnBnClickedButtonUpdate()
{
#ifdef DISABLE_QUICK_UPDATE
    if (!g_pBootloader->m_isConneted)
    {
        PrintLog(_T("No device is connected!\r\nTry to connect device first.\r\n"));
    }
#endif
    Bootloader::FlashFirmwareOption config;

    // Convert CString to std::string.
    CString wFullPath;
    m_ctrlImageFile.GetWindowTextW(wFullPath);
    CStringA tFullPath(wFullPath.GetBuffer());
    wFullPath.ReleaseBuffer();
    config.imageFilePath = tFullPath.GetBuffer();
    tFullPath.ReleaseBuffer();

    CString extension = PathFindExtension(wFullPath);
    if (extension.CompareNoCase(_T(".bin")) == 0)
    {
        if (!cstringtoui(m_address, config.baseAddress))
        {
            PrintLog(_T("Error: Invalid address input.(%s)"), m_address);
            return;
        }
    }
    else
    {
        config.baseAddress = 0;
    }

    CString memoryIdStr;
    m_comboMemoryID.GetWindowTextW(memoryIdStr);
    config.memoryId = g_pHistoryData->GetMemory(g_pHistoryData->FindMemory(memoryIdStr, NULL));

    config.doErasetoUnlock = m_doErasetoUnlock && (g_pBootloader->m_targetInfo.securityState != 0);
    config.useKeytoUnlock = m_useKeytoUnlock && (g_pBootloader->m_targetInfo.securityState != 0);
    CStringA tBackdoorkey(m_backdoorKey.GetBuffer());
    config.backdoorkey = tBackdoorkey.GetBuffer();

    // For normal update, kill timer when autocnt button is not checked.
    if (((CButton *)GetDlgItem(IDC_CHECK_AUTOCNT))->GetCheck() != BST_CHECKED)
    {
        g_pMainDlg->KillTimer(1);
    }
    else
    {
        g_pMainDlg->SetTimer(1, 1000, NULL);
    }

#ifndef DISABLE_QUICK_UPDATE
    if (g_pBootloader->m_isConneted)
    {
#endif
        g_pBootloader->UpdateImage(NULL, &config);
#ifndef DISABLE_QUICK_UPDATE
    }
    else
    {
        blfwkdll::Updater::PeripheralConfig peripheralConfig;
        g_pMainDlg->GetPeripheralConfig(&peripheralConfig);

        g_pBootloader->UpdateImage(&peripheralConfig, &config);
    }
#endif
}

// See UpdateDlg.h for documentation of this method.
LRESULT UpdateDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
    int percentage = m_progressCtrl.GetPos();

    if (percentage != (int)wParam)
    {
        percentage = (int)wParam;
        GetDlgItem(IDC_STATIC_PROGRESS_BAR)->SetWindowTextW(format_cstring(_T("%2d%%"), percentage));
        m_progressCtrl.SetPos(percentage);
    }

    return 0;
}
