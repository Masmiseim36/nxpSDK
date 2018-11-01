/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "afxwin.h"

// ConfigDlg Dialog
/*!
* \brief ConfigDlg class is the UI Dialog class for the
*  BCA utilities tab page
*/
class ConfigDlg : public CDialogEx
{
    DECLARE_DYNAMIC(ConfigDlg)

public:
    //! \brief Generated standard constructor.
    ConfigDlg(CWnd *pParent = NULL); // standard constructor

    //! \brief Generated standard destructor.
    virtual ~ConfigDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_DIALOG_CONFIG
    };
#endif

    //! \brief Generated message map functions.
protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnSelchangeComboFile();
    afx_msg void OnKillfocusComboFile();
    afx_msg void OnBnClickedButtonBrowse();
    afx_msg void OnChangeEditBcaData();
    afx_msg void OnBnClickedButtonConfigBca();
    afx_msg void OnBnClickedButtonSaveBca();

public:
    //! \brief  Refresh the image file list.
    void RefreshImageFileList();

    //! \brief  Read BCA data from image file to memory region(m_bcaData).
    BOOL ReadBootloaderConfigureArea();

    //! \brief  Update BCA data between memory region and UI
    //!
    //! \param direction    The update direction between UI and memory.
    //!                     TRUE:   from UI to memory
    //!                     FALSE:  from memory to UI
    void UpdateDataOfBootloaderConfigureArea(BOOL direction);

    //! \brief  Write BCA data to image file.
    void WriteBootloaderConfigureArea();

    //! \brief  Reset BCA data to all 0xFFs.
    void ResetBootloaderConfigureArea();

public:
    CComboBox m_ctrlImageFile; //!< A control variable for the Combox, IDC_COMBO_FILE2.
    CString m_bcaBinaries;     //!< A string variable for the Edit Control, IDC_EDIT_BCA_BINARY.
    char *m_bcaData;           //!< A pointer, pointing to a memory region that contains BCA binaries.
};