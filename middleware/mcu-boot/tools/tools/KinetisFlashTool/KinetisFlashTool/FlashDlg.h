/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "afxwin.h"
#include "../../common/blfwkdll/blfwkdll.h"

// FlashDlg Dialog
/*!
* \brief FlashDlg class is the UI Dialog class for the
*  Flash utilities tab page
*/
class FlashDlg : public CDialogEx
{
    DECLARE_DYNAMIC(FlashDlg)

public:
    //! \brief Generated standard constructor.
    FlashDlg(CWnd *pParent = NULL); // standard constructor

    //! \brief Generated standard destructor.
    virtual ~FlashDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_DIALOG_FLASH
    };
#endif

protected:
    //! \brief Generated message map functions.
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();

public:
    //! \brief Generated message map functions.
    virtual BOOL OnInitDialog();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnSelchangeComboEraseOpt();
    afx_msg void OnCbnSelchangeComboEraseMemoryid();
    afx_msg void OnBnClickedButtonErase();
    afx_msg void OnBnClickedButtonIfr();
    afx_msg void OnKillfocusComboEraseMemoryid();
    //! \brief  Refresh the memory id list.
    void RefreshMemoryIDList();

    CComboBox m_comboEraseOpt; //!< Control variable for erase option combox
    CComboBox m_comboMemoryID; //!< Control variable for memory option combox
};
