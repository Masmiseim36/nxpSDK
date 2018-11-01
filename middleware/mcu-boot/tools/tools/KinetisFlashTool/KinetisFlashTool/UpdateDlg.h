/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "../../common/blfwkdll/blfwkdll.h"

// UpdateDlg dialog
/*!
* \brief UpdateDlg class is the UI Dialog class for the
*  Update tab page
*/
class UpdateDlg : public CDialogEx
{
    DECLARE_DYNAMIC(UpdateDlg)

public:
    //! \brief Generated standard constructor.
    UpdateDlg(CWnd *pParent = NULL); // standard constructor

    //! \brief Generated standard destructor.
    virtual ~UpdateDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_DIALOG_UPDATE
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
    afx_msg void OnSelchangeComboFile();
    afx_msg void OnKillfocusComboFile();
    afx_msg void OnBnClickedButtonBrowse();
    afx_msg void OnSelchangeComboMemid();
    afx_msg void OnKillfocusComboMemid();
    afx_msg void OnBnClickedRadioErase();
    afx_msg void OnBnClickedRadioUnlock();
    afx_msg void OnChangeEdit();
    afx_msg void OnBnClickedButtonUpdate();
    afx_msg LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
    //! \brief  Refresh the image file list.
    void RefreshImageFileList();
    //! \brief  Refresh the memory id list.
    void RefreshMemoryIDList();

    CComboBox m_ctrlImageFile;    //!< A control variable for the Combox, IDC_COMBO_FILE.
    CString m_address;            //!< A control variable for the Edit Control, IDC_EDIT_ADDRESS.
    CString m_backdoorKey;        //!< A control variable for the Edit Control, IDC_EDIT_KEY.
    BOOL m_doErasetoUnlock;       //!< A variable for whether do flash-erase-all-unsecure command when updating.
    BOOL m_useKeytoUnlock;        //!< A variable for whether do flash-security-disable command when updating.
    CProgressCtrl m_progressCtrl; //!< A control variable for the progress bar
    CComboBox m_comboMemoryID;    //!< A control variable for the Combox, IDC_COMBO_MEMID
};
