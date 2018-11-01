/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

// KinetisFlashToolDlg.h : header file
//
#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include <Setupapi.h>
#include "UpdateDlg.h"
#include "FlashDlg.h"
#include "ConfigDlg.h"
#include "../../common/blfwkdll/blfwkdll.h"

// class CKinetisFlashToolDlgAutoProxy;

// CKinetisFlashToolDlg dialog
/*!
* \brief CKinetisFlashToolDlg class is the UI Dialog class for the main page
*/
class CKinetisFlashToolDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CKinetisFlashToolDlg);
    // friend class CKinetisFlashToolDlgAutoProxy;

    // Construction
public:
    //! \brief Generated standard constructor.
    CKinetisFlashToolDlg(CWnd *pParent = NULL); // standard constructor

    //! \brief Generated standard destructor.
    virtual ~CKinetisFlashToolDlg();

    // Dialog Data
    enum
    {
        IDD = IDD_KINETISFLASHTOOL_DIALOG
    };

protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    // Implementation
protected:
    // CKinetisFlashToolDlgAutoProxy* m_pAutoProxy;
    HICON m_hIcon;

    BOOL CanExit();

    //! \brief Generated message map functions.
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnClose();
    virtual void OnOK();
    virtual void OnCancel();
    DECLARE_MESSAGE_MAP()

    //! \brief  Scan COM port and update it to UI.
    void RefreshComPortList();

    //! \brief  Refresh baud rate list.
    void RefreshBaudrateList();

    //! \brief  Refresh PID list.
    void RefreshPidList();

    //! \brief  Refresh VID list.
    void RefreshVidList();

    //! \brief  Append a new log string to the log textbox.
    //!
    //! \param  Log string.
    void AppendLog(CString log);

    //! \brief  Reset operation.
    void Reset();

    //! \brief  Connect operation.
    void Connect();

public:
    //! \brief Generated message map functions.
    afx_msg void OnBnClickedRadio();
    afx_msg void OnCbnSelchangeComboPortVid();
    afx_msg void OnCbnSelchangeComboBaudPid();
    afx_msg void OnKillfocusComboPortVid();
    afx_msg void OnKillfocusComboBaudPid();
    afx_msg void OnClickedButtonCntReset();
    afx_msg void OnTcnSelchangeTabFunc(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);

    //! \brief User defined message map functions.
    afx_msg LRESULT OnUpdateLog(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnChangeConnection(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);

    //! \brief Get the PeripheralConfig of bootloader.
    void GetPeripheralConfig(blfwkdll::Updater::PeripheralConfig *config);

    //! \brief Register usb device interface.
    void RegisterUsbDeviceInterface();

    CComboBox m_comboPortOrVID;      //!< Control variable for Com port/VID combox
    CComboBox m_comboBaudOrPID;      //!< Control variable for Baud rate/PID combox
    CString m_bootloaderVersion;     //!< Variable for device info
    CString m_protoclVersion;        //!< Variable for device info
    CString m_securityState;         //!< Variable for device info
    CString m_flashSize;             //!< Variable for device info
    CString m_ramSize;               //!< Variable for device info
    CString m_rsvRgnFlashStart;      //!< Variable for device info
    CString m_rsvRgnFlashEnd;        //!< Variable for device info
    CString m_rsvRgnRamStart;        //!< Variable for device info
    CString m_rsvRgnRamEnd;          //!< Variable for device info
    CTabCtrl m_tabFunc;              //!< Control variable for tab control.
    CString m_log;                   //!< Variable for log text box.
    CEdit m_logCtrl;                 //!< Control variable for log textbox.
    UpdateDlg m_updateDlg;           //!< Variable for update tab page.
    FlashDlg m_flashDlg;             //!< Variable for flash utility tab page.
    ConfigDlg m_bootloaderConfigDLg; //!< Variable for config tab page.
    HDEVNOTIFY m_devNotify;          //!< Handler for device change notify.
};