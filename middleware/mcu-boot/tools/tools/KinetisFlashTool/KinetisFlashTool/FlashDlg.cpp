/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include "KinetisFlashTool.h"
#include "FlashDlg.h"
#include "HistoryData.h"
#include "afxdialogex.h"
#include "Bootloader.h"
#include "utility.h"
// FlashDlg dialog

IMPLEMENT_DYNAMIC(FlashDlg, CDialogEx)

#define ERASE_REGION (_T("Erase Region"))
#define ERASE_ALL (_T("Erase All"))
#define ERASE_ALL_UNSECURE (_T("Erase All and Unsecure"))

enum
{
    kIndex_eraseRegion = 0,
    kIndex_eraseAll = 1,
    kIndex_eraseAllUnsecure = 2,
};

extern void PrintLog(const wchar_t *format, ...);

// See FlashDlg.h for documentation of this method.
FlashDlg::FlashDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_FLASH, pParent)
{
}

// See FlashDlg.h for documentation of this method.
FlashDlg::~FlashDlg()
{
}

// See FlashDlg.h for documentation of this method.
void FlashDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ERASE_OPT, m_comboEraseOpt);
    DDX_Control(pDX, IDC_COMBO_ERASE_MEMORYID, m_comboMemoryID);
}

BEGIN_MESSAGE_MAP(FlashDlg, CDialogEx)
ON_CBN_SELCHANGE(IDC_COMBO_ERASE_OPT, &FlashDlg::OnSelchangeComboEraseOpt)
ON_BN_CLICKED(IDC_BUTTON_ERASE, &FlashDlg::OnBnClickedButtonErase)
ON_BN_CLICKED(IDC_BUTTON_IFR, &FlashDlg::OnBnClickedButtonIfr)
ON_CBN_SELCHANGE(IDC_COMBO_ERASE_MEMORYID, &FlashDlg::OnCbnSelchangeComboEraseMemoryid)
ON_WM_SHOWWINDOW()
ON_CBN_KILLFOCUS(IDC_COMBO_ERASE_MEMORYID, &FlashDlg::OnKillfocusComboEraseMemoryid)
END_MESSAGE_MAP()

// See ConfigDlg.h for documentation of this method.
void FlashDlg::OnOK()
{
    // CDialogEx::OnOK();
}

// See ConfigDlg.h for documentation of this method.
void FlashDlg::OnCancel()
{
    // CDialogEx::OnCancel();
}

// See FlashDlg.h for documentation of this method.
BOOL FlashDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    // Init the combo boxes
    m_comboEraseOpt.AddString(ERASE_REGION);
    m_comboEraseOpt.AddString(ERASE_ALL);
    m_comboEraseOpt.AddString(ERASE_ALL_UNSECURE);
    m_comboEraseOpt.SetCurSel(kIndex_eraseRegion);

    RefreshMemoryIDList();

    // Init IFR program sequnce check box.
    ((CButton *)GetDlgItem(IDC_CHECK_IFR_MSB))->SetCheck(BST_UNCHECKED);

    return TRUE; // return TRUE unless you set the focus to a control
}

// See FlashDlg.h for documentation of this method.
void FlashDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialogEx::OnShowWindow(bShow, nStatus);

    if (bShow)
    {
        RefreshMemoryIDList();
    }
}

// See FlashDlg.h for documentation of this method.
void FlashDlg::RefreshMemoryIDList()
{
    int eraseOpt = m_comboEraseOpt.GetCurSel();

    if (eraseOpt != kIndex_eraseAllUnsecure)
    {
        // Clear the content.
        m_comboMemoryID.ResetContent();

        for (size_t i = 0; i < g_pHistoryData->GetMemoryCount(); i++)
        {
            if ((eraseOpt == kIndex_eraseAll) ||
                (blfwkdll::Updater::IsMemorySpaceId(g_pHistoryData->GetMemory(i).first)))
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
}

// See FlashDlg.h for documentation of this method.
void FlashDlg::OnSelchangeComboEraseOpt()
{
    int eraseOpt = m_comboEraseOpt.GetCurSel();

    // Memory ID combox only is enabled when erase-all command is selected.
    GetDlgItem(IDC_STATIC_ERASE_ID)->EnableWindow(eraseOpt != kIndex_eraseAllUnsecure);
    GetDlgItem(IDC_COMBO_ERASE_MEMORYID)->EnableWindow(eraseOpt != kIndex_eraseAllUnsecure);

    // address and length only is enabled when erase-region command is selected.
    GetDlgItem(IDC_STATIC_ERASE_ADDR)->EnableWindow(eraseOpt == kIndex_eraseRegion);
    GetDlgItem(IDC_EDIT_ERASE_ADDRESS)->EnableWindow(eraseOpt == kIndex_eraseRegion);
    GetDlgItem(IDC_STATIC_ERASE_LENGTH)->EnableWindow(eraseOpt == kIndex_eraseRegion);
    GetDlgItem(IDC_EDIT_ERASE_LENGTH)->EnableWindow(eraseOpt == kIndex_eraseRegion);

    RefreshMemoryIDList();
}

// See FlashDlg.h for documentation of this method.
void FlashDlg::OnCbnSelchangeComboEraseMemoryid()
{
    CString memoryIdStr;
    m_comboMemoryID.GetLBText(m_comboMemoryID.GetCurSel(), memoryIdStr);
    g_pHistoryData->SetCurrentMemoryId(g_pHistoryData->FindMemory(memoryIdStr, NULL));
    RefreshMemoryIDList();
}

// See FlashDlg.h for documentation of this method.
void FlashDlg::OnKillfocusComboEraseMemoryid()
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

// See FlashDlg.h for documentation of this method.
void FlashDlg::OnBnClickedButtonErase()
{
    if (!g_pBootloader->m_isConneted)
    {
        PrintLog(_T("No device is connected!\r\nPlease press Connect button to connect the device first.\r\n"));
        return;
    }

    Bootloader::EraseOption option;

    int eraseOpt = m_comboEraseOpt.GetCurSel();

    if (eraseOpt == kIndex_eraseRegion) // erase region
    {
        option.option = Bootloader::EraseOption::ERASE_OPERATION::erase_region;

        CString startAddr;
        GetDlgItem(IDC_EDIT_ERASE_ADDRESS)->GetWindowTextW(startAddr);
        if (!cstringtoui(startAddr, option.startAddress))
        {
            PrintLog(_T("Error: Invalid Start Address.(%s)"), startAddr);
            return;
        }
        CString length;
        GetDlgItem(IDC_EDIT_ERASE_LENGTH)->GetWindowTextW(length);
        if (!cstringtoui(length, option.length))
        {
            PrintLog(_T("Error: Invalid Length.(%s)"), length);
            return;
        }
        CString memoryIdStr;
        m_comboMemoryID.GetWindowTextW(memoryIdStr);
        option.memoryId = g_pHistoryData->GetMemory(g_pHistoryData->FindMemory(memoryIdStr, NULL));
    }
    else if (eraseOpt == kIndex_eraseAll) // flash-erase-all
    {
        CString memoryIdStr;
        m_comboMemoryID.GetWindowTextW(memoryIdStr);
        option.option = Bootloader::EraseOption::ERASE_OPERATION::erase_all;
        option.memoryId = g_pHistoryData->GetMemory(g_pHistoryData->FindMemory(memoryIdStr, NULL));
    }
    else if (eraseOpt == kIndex_eraseAllUnsecure) // flash-erase-all-unsecure
    {
        option.option = Bootloader::EraseOption::ERASE_OPERATION::erase_all_and_unsecure;
    }
    g_pBootloader->Erase(&option);
}

// See FlashDlg.h for documentation of this method.
void FlashDlg::OnBnClickedButtonIfr()
{
    if (!g_pBootloader->m_isConneted)
    {
        PrintLog(_T("No device is connected!\r\nPlease press Connect button to connect the device first.\r\n"));
        return;
    }

    Bootloader::ProgramOnceOption option;

    CString strW;
    GetDlgItem(IDC_EDIT_IFR_INDEX)->GetWindowTextW(strW);
    if (!cstringtoui(strW, option.index))
    {
        PrintLog(_T("Error: Invalid index.(%s)"), strW);
        return;
    }

    GetDlgItem(IDC_EDIT_IFR_BYTECOUNT)->GetWindowTextW(strW);
    if (!cstringtoui(strW, option.byteCount))
    {
        PrintLog(_T("Error: Invalid byteCount.(%s)"), strW);
        return;
    }

    if (((CButton *)GetDlgItem(IDC_CHECK_IFR_MSB))->GetCheck() == BST_CHECKED)
    {
        option.isLsb = false;
    }
    else
    {
        option.isLsb = true;
    }

    // Convert WCHAR to CHAR
    GetDlgItem(IDC_EDIT_IFR_DATA)->GetWindowTextW(strW);
    CStringA strA(strW.GetBuffer());
    strW.ReleaseBuffer();
    option.data = strA.GetBuffer();
    strA.ReleaseBuffer();

    g_pBootloader->ProgramOnce(&option);
}
