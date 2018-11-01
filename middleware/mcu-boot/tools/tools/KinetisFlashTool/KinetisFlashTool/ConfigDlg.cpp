/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include "KinetisFlashTool.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"

#include "HistoryData.h"
#include "utility.h"
#include "BootloaderConfigDlg.h"

extern void PrintLog(const wchar_t *format, ...);
// ConfigDlg dialog

IMPLEMENT_DYNAMIC(ConfigDlg, CDialogEx)

// See ConfigDlg.h for documentation of this method.
ConfigDlg::ConfigDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_CONFIG, pParent)
    , m_bcaData(NULL)
    , m_bcaBinaries(_T(""))
{
}

// See ConfigDlg.h for documentation of this method.
ConfigDlg::~ConfigDlg()
{
    if (m_bcaData != NULL)
    {
        free(m_bcaData);
    }
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_FILE2, m_ctrlImageFile);
    DDX_Text(pDX, IDC_EDIT_BCA_BINARY, m_bcaBinaries);
}

BEGIN_MESSAGE_MAP(ConfigDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON_BROWSE2, &ConfigDlg::OnBnClickedButtonBrowse)
ON_CBN_SELCHANGE(IDC_COMBO_FILE2, &ConfigDlg::OnSelchangeComboFile)
ON_BN_CLICKED(IDC_BUTTON_SAVE_BCA, &ConfigDlg::OnBnClickedButtonSaveBca)
ON_EN_CHANGE(IDC_EDIT_BCA_BINARY, &ConfigDlg::OnChangeEditBcaData)
ON_WM_SHOWWINDOW()
ON_BN_CLICKED(IDC_BUTTON_CONFIG_BCA, &ConfigDlg::OnBnClickedButtonConfigBca)
ON_CBN_KILLFOCUS(IDC_COMBO_FILE2, &ConfigDlg::OnKillfocusComboFile)
END_MESSAGE_MAP()

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnOK()
{
    // CDialogEx::OnOK();
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnCancel()
{
    // CDialogEx::OnCancel();
}

// See ConfigDlg.h for documentation of this method.
BOOL ConfigDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO: Add extra initialization here
    if (m_bcaData == NULL)
    {
        m_bcaData = (char *)malloc(64);
        FillMemory(m_bcaData, 64, 0xFF);
    }
    // 128 for BCA characters,
    // There are four lines of BCA characters
    // 2*4 for four "\r\n" pairs.
    ((CEdit *)GetDlgItem(IDC_EDIT_BCA_BINARY))->SetLimitText(128 + 2 * 4);

    // RefreshImageFileList();

    UpdateData(FALSE);
    return TRUE; // return TRUE unless you set the focus to a control
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::RefreshImageFileList()
{
    // Clear the contents
    m_ctrlImageFile.ResetContent();

    for (size_t i = 0; i < g_pHistoryData->GetFileCount(); i++)
    {
        m_ctrlImageFile.AddString(g_pHistoryData->GetFilePath(i));
    }

    bool enableSave = false;
    if (g_pHistoryData->GetFileCount() > 0)
    {
        m_ctrlImageFile.SetCurSel(0);
        if (g_pHistoryData->GetFileType(0) != HistoryData::FileType::kBinary)
        {
            PrintLog(_T("Warning: Currently, only binary file is supported!\r\n"));
            enableSave = false;
        }
        else if (ReadBootloaderConfigureArea())
        {
            enableSave = true;
        }
    }
    else
    {
        enableSave = false;
    }
    if (!enableSave)
    {
        ResetBootloaderConfigureArea();
    }
    GetDlgItem(IDC_BUTTON_SAVE_BCA)->EnableWindow(enableSave);
}

// See ConfigDlg.h for documentation of this method.
BOOL ConfigDlg::ReadBootloaderConfigureArea()
{
    CString filePath;
    m_ctrlImageFile.GetWindowTextW(filePath);
    CFile *cFile = new CFile(filePath.GetBuffer(), CFile::modeReadWrite | CFile::typeBinary);
    filePath.ReleaseBuffer();

    ULONGLONG length = cFile->GetLength();
    if (length < 0x400)
    {
        PrintLog(_T("Warning: File(%s) is smaller than 1K bytes, fill 0xFFs to extend it to 1K bytes."), filePath);
        cFile->SeekToEnd();
        while (length < 0x400)
        {
            cFile->Write("\xFF", 1);
            length++;
        }
        cFile->Flush(); // Flush the 0xFFs to file.
    }

    cFile->Seek(0x3C0, CFile::begin);
    cFile->Read(m_bcaData, 64);

    PrintLog(_T("Read BCA data from image file."));

    delete cFile;

    UpdateDataOfBootloaderConfigureArea(FALSE);

    return TRUE;
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::UpdateDataOfBootloaderConfigureArea(BOOL direction)
{
    if (direction)
    {
        UpdateData(TRUE); // Update UI to CString m_bcaBinaries

        int length = m_bcaBinaries.GetLength();
        LPWSTR buffer = m_bcaBinaries.GetBuffer();

        char tempHigh, tempLow;
        int posBin = 0, posChar = 0;
        while ((posBin < 64) && (posChar < length))
        {
            // Only convert legal characters.
            if (isxdigit(buffer[posChar * 2]) && isxdigit(buffer[posChar * 2 + 1]))
            {
                tempHigh = buffer[posChar * 2] & 0xFF;    // change TCHAR to char, abandon the high bits.
                tempLow = buffer[posChar * 2 + 1] & 0xFF; // change TCHAR to char, abandon the high bits.
                m_bcaData[posBin] = ((hex_to_int(tempHigh) & 0xFF) << 4) | (hex_to_int(tempLow) & 0xFF);
                posBin++;
                posChar++;
            }
            else
            {
                // Skip the illegal character pairs.
                posChar++;
            }
        }
        m_bcaBinaries.ReleaseBuffer();
    }
    else
    {
        LPTSTR buffer = m_bcaBinaries.GetBuffer(128 + 2 * 4 + 1); // 128 for 128 BCA characters
                                                                  // 2 * 4 for enter characters("\r\n") each line
                                                                  // 1 byte more for for terminator("\0")
        int posBin = 0, posChar = 0;
        for (; posBin < 64; posChar++, posBin++)
        {
            // 32 characters each line.
            if ((!(posBin % 0x10)) && (posBin != 0))
            {
                // Add "\r\n" at the end of each line
                buffer[posChar * 2] = '\r';
                buffer[posChar * 2 + 1] = '\n';
                posChar++;
            }
            buffer[posChar * 2] = int_to_hex(m_bcaData[posBin] >> 4);
            buffer[posChar * 2 + 1] = int_to_hex(m_bcaData[posBin]);
        }
        buffer[posChar * 2] = '\0';
        m_bcaBinaries.ReleaseBuffer();

        UpdateData(FALSE); // Update CString m_bcaBinaries to UI
    }
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::WriteBootloaderConfigureArea()
{
    UpdateDataOfBootloaderConfigureArea(TRUE);

    CString filePath;
    m_ctrlImageFile.GetWindowTextW(filePath);
    CFile *cFile = new CFile(filePath.GetBuffer(), CFile::modeWrite | CFile::typeBinary);
    filePath.ReleaseBuffer();

    cFile->Seek(0x3C0, CFile::begin);
    cFile->Write(m_bcaData, 64);
    cFile->Flush();

    PrintLog(_T("Write new BCA data to image file."));
    delete (cFile);
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::ResetBootloaderConfigureArea()
{
    if (m_bcaData == NULL)
    {
        m_bcaData = (char *)malloc(64);
    }
    FillMemory(m_bcaData, 64, 0xFF);
    UpdateDataOfBootloaderConfigureArea(FALSE);
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialogEx::OnShowWindow(bShow, nStatus);

    // Image file list might be changed at other tab page.
    // Refresh it when showing this tab page.
    if (bShow)
    {
        RefreshImageFileList();
    }
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnBnClickedButtonBrowse()
{
    CFileDialog cFileDlg(TRUE);

    cFileDlg.m_ofn.lpstrTitle = _T("Select the image file");
    cFileDlg.m_ofn.lpstrFilter = _T("Supported files(*.bin)\0*.bin;\0All files(*.*)\0*.*\0\0");
    if (IDOK == cFileDlg.DoModal())
    {
        CString fullPath = cFileDlg.GetPathName();
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

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnChangeEditBcaData()
{
    // Update UI to string variable;
    UpdateData(TRUE);
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnSelchangeComboFile()
{
    // If a file is selected, bubble it to the head of a file list.
    g_pHistoryData->BubbleSelectedFile(m_ctrlImageFile.GetCurSel());
    RefreshImageFileList();
}

void ConfigDlg::OnKillfocusComboFile()
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
        m_ctrlImageFile.SetCurSel(-1);                        // Set invalid selection.
        ResetBootloaderConfigureArea();                       // Reset configureArea to 0xFFs.
        GetDlgItem(IDC_BUTTON_SAVE_BCA)->EnableWindow(false); // Disable save button.
    }
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnBnClickedButtonConfigBca()
{
    // BCA data in the UI page might be changed.
    // Update it to memory region.
    UpdateDataOfBootloaderConfigureArea(TRUE);

    CString filePath;
    m_ctrlImageFile.GetWindowTextW(filePath);

    CFile *cFile = new CFile;
    if (!cFile->Open(filePath, CFile::modeRead | CFile::typeBinary, NULL))
    {
        // If cannot open this file, transfer NULL to bootloader config dlg.
        delete cFile;
        cFile = NULL;
    }
    else
    {
        // Check if this file is binary file.
        CString extension = PathFindExtension(filePath);
        if (extension.CompareNoCase(_T(".bin")) != 0)
        {
            // If not binary file, close it and transfer NULL to bootloader config dlg.
            cFile->Close();
            delete cFile;
            cFile = NULL;
        }
    }

    BootloaderConfigDlg dlg((blfwkdll::BootloaderConfigureAreaData *)m_bcaData, cFile, this);

    dlg.DoModal();

    // Update the new BCA to UI page.
    UpdateDataOfBootloaderConfigureArea(FALSE);

    if (cFile != NULL)
    {
        cFile->Close();
        delete cFile;
        cFile = NULL;
    }
}

// See ConfigDlg.h for documentation of this method.
void ConfigDlg::OnBnClickedButtonSaveBca()
{
    WriteBootloaderConfigureArea();
}
