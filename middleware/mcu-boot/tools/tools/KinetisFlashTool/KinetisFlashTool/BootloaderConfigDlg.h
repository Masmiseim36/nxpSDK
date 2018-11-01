/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "../../common/blfwkdll/blfwkdll.h"

// BootloaderConfigDlg dialog
/*!
* \brief BootloaderConfigDlg class is the UI Dialog class for the
*  bootloader configuration page
*/
class BootloaderConfigDlg : public CDialogEx
{
    DECLARE_DYNAMIC(BootloaderConfigDlg)

public:
    //! \brief Generated standard constructor.
    BootloaderConfigDlg(CWnd *pParent = NULL); // standard constructor

    //! \brief Custom constructor
    //!
    //! \param  config      A pointer to BCA data memory.
    //!                     (Must be specified)
    //!
    //! \param  file        A file pointer to the binary image, used for CRC calculation.
    //!                     (Must be specified.)
    //!
    //! \param  pParent     the father dialog pointer.
    BootloaderConfigDlg(blfwkdll::BootloaderConfigureAreaData *config, CFile *file, CWnd *pParent = NULL)
        : CDialogEx(IDD_DIALOG_BOOTLOADERCONFIG, pParent)
    {
        m_config = config;
        m_cFile = file;
    };

    //! \brief Generated standard destructor.
    virtual ~BootloaderConfigDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_DIALOG_BOOTLOADERCONFIG
    };
#endif
    //! \brief Generated message map functions.
protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedCheckCrc();
    afx_msg void OnBnClickedCheckI2cslaveAddress();
    afx_msg void OnBnClickedCheckTimeout();
    afx_msg void OnBnClickedCheckVid();
    afx_msg void OnBnClickedCheckPid();
    afx_msg void OnBnClickedCheckUsbstringPointer();
    afx_msg void OnBnClickedCheckDisableHighspeed();
    afx_msg void OnBnClickedCheckMmcauPointer();
    afx_msg void OnBnClickedCheckOtfadPointer();
    afx_msg void OnBnClickedCheckQspiPointer();
    afx_msg void OnBnClickedCheckCantxid();
    afx_msg void OnBnClickedCheckCanrxid();
    afx_msg void OnBnClickedCheckCanspeed();
    afx_msg void OnBnClickedRadioSpeedindex();
    afx_msg void OnBnClickedReload();
    afx_msg void OnBnClickedButtonConfigOk();
    afx_msg void OnBnClickedButtonConfigCancel();
    afx_msg void OnBnClickedButtonConfigGenerate();

public:
    //! \brief  Load BCA data to UI page.
    void LoadBootloaderConfigureArea();

    //! \brief  Save UI config data to BCA.
    //!
    //! \return OnOK for success, OnCancel for failure.
    int SaveBootloaderConfigureArea();

protected:
    blfwkdll::BootloaderConfigureAreaData *m_config; //!< A pointer to BCA data.
    CFile *m_cFile;                                  //!< A pointer to the binary file, used for crc calculation.
};
