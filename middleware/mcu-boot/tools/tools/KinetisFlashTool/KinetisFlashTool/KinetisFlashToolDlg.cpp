/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

// KinetisFlashToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KinetisFlashTool.h"
#include "KinetisFlashToolDlg.h"
#include "afxdialogex.h"
#include "SerialList.h"
#include "HistoryData.h"
#include "Bootloader.h"
#include "utility.h"
#include <WinUser.h>
#include <Dbt.h>
#include <cfgmgr32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CKinetisFlashToolDlg *g_pMainDlg;

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
    //! \brief Generated standard constructor.
    CAboutDlg();

    // Dialog Data
    enum
    {
        IDD = IDD_ABOUTBOX
    };

protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    // Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg()
    : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CKinetisFlashToolDlg dialog

IMPLEMENT_DYNAMIC(CKinetisFlashToolDlg, CDialogEx);
// See CKinetisFlashToolDlg.h for documentation of this method.
CKinetisFlashToolDlg::CKinetisFlashToolDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(CKinetisFlashToolDlg::IDD, pParent)
    , m_log(_T(""))
    , m_bootloaderVersion(_T(""))
    , m_protoclVersion(_T(""))
    , m_securityState(_T(""))
    , m_flashSize(_T(""))
    , m_ramSize(_T(""))
    , m_rsvRgnFlashStart(_T(""))
    , m_rsvRgnFlashEnd(_T(""))
    , m_rsvRgnRamStart(_T(""))
    , m_rsvRgnRamEnd(_T(""))
    , m_devNotify(NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_FREESCALE);
}

// See CKinetisFlashToolDlg.h for documentation of this method.
CKinetisFlashToolDlg::~CKinetisFlashToolDlg()
{
    // If there is an automation proxy for this dialog, set
    //  its back pointer to this dialog to NULL, so it knows
    //  the dialog has been deleted.
    // if (m_pAutoProxy != NULL)
    //	m_pAutoProxy->m_pDialog = NULL;
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_FUNC, m_tabFunc);
    DDX_Control(pDX, IDC_COMBO_PORT_VID, m_comboPortOrVID);
    DDX_Control(pDX, IDC_COMBO_BAUD_PID, m_comboBaudOrPID);
    DDX_Text(pDX, IDC_EDIT_LOG, m_log);
    DDX_Control(pDX, IDC_EDIT_LOG, m_logCtrl);
    DDX_Text(pDX, IDC_STATIC_BOOTLLOADER_VERSION, m_bootloaderVersion);
    DDX_Text(pDX, IDC_STATIC_SERIAL_VERSION, m_protoclVersion);
    DDX_Text(pDX, IDC_STATIC_SECURITY_STATE, m_securityState);
    DDX_Text(pDX, IDC_STATIC_FLASH_SIZE, m_flashSize);
    DDX_Text(pDX, IDC_STATIC_RAM_SIZE, m_ramSize);
    DDX_Text(pDX, IDC_STATIC_RESERVED_FLASH_START, m_rsvRgnFlashStart);
    DDX_Text(pDX, IDC_STATIC_RESERVED_FLASH_END, m_rsvRgnFlashEnd);
    DDX_Text(pDX, IDC_STATIC_RESERVED_RAM_START, m_rsvRgnRamStart);
    DDX_Text(pDX, IDC_STATIC_RESERVED_RAM_END, m_rsvRgnRamEnd);
}

BEGIN_MESSAGE_MAP(CKinetisFlashToolDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_CLOSE()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_RADIO_UART, &CKinetisFlashToolDlg::OnBnClickedRadio)
ON_BN_CLICKED(IDC_RADIO_USBHID, &CKinetisFlashToolDlg::OnBnClickedRadio)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FUNC, &CKinetisFlashToolDlg::OnTcnSelchangeTabFunc)
ON_CBN_SELCHANGE(IDC_COMBO_PORT_VID, &CKinetisFlashToolDlg::OnCbnSelchangeComboPortVid)
ON_CBN_SELCHANGE(IDC_COMBO_BAUD_PID, &CKinetisFlashToolDlg::OnCbnSelchangeComboBaudPid)
ON_CBN_KILLFOCUS(IDC_COMBO_PORT_VID, &CKinetisFlashToolDlg::OnKillfocusComboPortVid)
ON_CBN_KILLFOCUS(IDC_COMBO_BAUD_PID, &CKinetisFlashToolDlg::OnKillfocusComboBaudPid)
ON_BN_CLICKED(IDC_BUTTON_CNT_RESET, &CKinetisFlashToolDlg::OnClickedButtonCntReset)
ON_WM_TIMER()
ON_WM_DEVICECHANGE()
// User defined message
ON_MESSAGE(USER_MS_UPDATA_LOG, &CKinetisFlashToolDlg::OnUpdateLog)
ON_MESSAGE(USER_MS_CONNECTION_CHANGE, &CKinetisFlashToolDlg::OnChangeConnection)
ON_MESSAGE(USER_MS_UPDATA_STATUS, &CKinetisFlashToolDlg::OnUpdateStatus)
END_MESSAGE_MAP()

// CKinetisFlashToolDlg message handlers

// See CKinetisFlashToolDlg.h for documentation of this method.
BOOL CKinetisFlashToolDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // ShowWindow(SW_MAXIMIZE);

    // TODO: Add extra initialization here
    g_pMainDlg = this;

    // Init COM port and baudrate list
    RefreshComPortList();
    RefreshBaudrateList();
    // Init port button. uart is selected as default
    ((CButton *)GetDlgItem(IDC_RADIO_UART))->SetCheck(true);
    ((CButton *)GetDlgItem(IDC_RADIO_USBHID))->SetCheck(false);

    ((CEdit *)GetDlgItem(IDC_STATIC_PORT_VID))->SetWindowTextW(_T("Port:"));
    ((CEdit *)GetDlgItem(IDC_STATIC_BAUD_PID))->SetWindowTextW(_T("Baud:"));

    // Register usb device notification.
    RegisterUsbDeviceInterface();

    // Init tab page.
    // Add tab pages
    m_tabFunc.InsertItem(0, _T("Update"));
    m_tabFunc.InsertItem(1, _T("Flash Utilities"));
    m_tabFunc.InsertItem(2, _T("BCA Utilities"));
    // m_tabFunc.InsertItem(3, _T("IFR Utilities"));
    // m_tabFunc.InsertItem(4, _T("Blhost Utilities"));
    // Get tab page's size, and calculate child page's size.
    CRect rc;
    m_tabFunc.GetClientRect(&rc);
    rc.top += 22; // add 20 more to discover the tab title.
    rc.bottom -= 2;
    rc.left += 2;
    rc.right -= 2;
    // Setup update page
    m_updateDlg.Create(IDD_DIALOG_UPDATE, GetDlgItem(IDC_TAB_FUNC)); // create update page
    m_updateDlg.SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                             SWP_SHOWWINDOW); // set window position
    m_updateDlg.ShowWindow(true);
    // Setup Flash utilities page
    m_flashDlg.Create(IDD_DIALOG_FLASH, GetDlgItem(IDC_TAB_FUNC)); // create config page
    m_flashDlg.SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                            SWP_SHOWWINDOW); // set window position
    m_flashDlg.ShowWindow(false);
    // Setup BCA utilities page
    m_bootloaderConfigDLg.Create(IDD_DIALOG_CONFIG, GetDlgItem(IDC_TAB_FUNC)); // create config page
    m_bootloaderConfigDLg.SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                                       SWP_SHOWWINDOW); // set window position
    m_bootloaderConfigDLg.ShowWindow(false);

    m_tabFunc.SetCurSel(0); // set default tab page to page 0(Update page)

    return TRUE; // return TRUE  unless you set the focus to a control
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKinetisFlashToolDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user
// drags
//  the minimized window.
HCURSOR CKinetisFlashToolDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CKinetisFlashToolDlg::OnClose()
{
    uint32_t address = 0;
    if (cstringtoui(m_updateDlg.m_address, address))
    {
        g_pHistoryData->SetImageAddress(address);
    }
    g_pHistoryData->SetBackdoorKey(m_updateDlg.m_backdoorKey);

    UnregisterDeviceNotification(m_devNotify);
    /*
    // Reset command will be sent when bootloader thread exits, not here.
    if (g_pBootloader->m_isConneted)
    {
        Reset();
    }*/
    if (CanExit())
        CDialogEx::OnClose();
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnOK()
{
    // if (CanExit())
    //    CDialogEx::OnOK();
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnCancel()
{
    if (CanExit())
        CDialogEx::OnCancel();
}

// See CKinetisFlashToolDlg.h for documentation of this method.
BOOL CKinetisFlashToolDlg::CanExit()
{
    // If the proxy object is still around, then the automation
    //  controller is still holding on to this application.  Leave
    //  the dialog around, but hide its UI.
    // if (m_pAutoProxy != NULL)
    //{
    //    ShowWindow(SW_HIDE);
    //    return FALSE;
    //}

    return TRUE;
}

void CKinetisFlashToolDlg::RegisterUsbDeviceInterface()
{
    /* The DBT_DEVICEARRIVAL and DBT_DEVICEREMOVECOMPLETE events are automatically broadcast
    * to all top-level windows for port devices. Therefore, it is not necessary to call
    * RegisterDeviceNotification for ports.
    */
    DEV_BROADCAST_DEVICEINTERFACE notificationFilter;
    ZeroMemory(&notificationFilter, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    notificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;

    m_devNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &notificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
    if (m_devNotify == NULL)
    {
        AppendLog(_T("Error: Failed to register COM port device notification"));
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::RefreshComPortList()
{
    m_comboPortOrVID.ResetContent(); // Clear the content in the combox

    // List all
    for (size_t i = 0; i < g_pAllComDevices->GetDeviceCount(); i++)
    {
        m_comboPortOrVID.AddString(g_pAllComDevices->GetDevice(i).GetString());
    }

    if (m_comboPortOrVID.GetCount() == 0)
    {
        AppendLog(_T("Cannot find a COM port!"));
    }
    else
    {
        m_comboPortOrVID.SetCurSel(g_pHistoryData->GetCurrentDevice());
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::RefreshBaudrateList()
{
    m_comboBaudOrPID.ResetContent(); // Clear the content in the combox

    // List all
    for (size_t i = 0; i < g_pHistoryData->GetBaudRateCount(); i++)
    {
        m_comboBaudOrPID.AddString(format_cstring(_T("%d"), g_pHistoryData->GetBaudRate(i)));
    }
    m_comboBaudOrPID.SetCurSel(g_pHistoryData->GetCurrentBaudRate());
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::RefreshPidList()
{
    m_comboBaudOrPID.ResetContent(); // Clear the content in the combox

    // List all
    for (size_t i = 0; i < g_pHistoryData->GetPidCount(); i++)
    {
        m_comboBaudOrPID.AddString(format_cstring(_T("0x%04X"), g_pHistoryData->GetPid(i)));
    }
    m_comboBaudOrPID.SetCurSel(0);
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::RefreshVidList()
{
    m_comboPortOrVID.ResetContent(); // Clear the content in the combox

    // List all
    for (size_t i = 0; i < g_pHistoryData->GetVidCount(); i++)
    {
        m_comboPortOrVID.AddString(format_cstring(_T("0x%04X"), g_pHistoryData->GetVid(i)));
    }
    m_comboPortOrVID.SetCurSel(0);
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::AppendLog(CString log)
{
    m_log.Append(log);
    m_log.Append(_T("\r\n"));
    UpdateData(false);
    ::PostMessage(m_logCtrl.m_hWnd, WM_VSCROLL, SB_BOTTOM, 0); // Keep Log page always shows the bottom of the log.
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::GetPeripheralConfig(blfwkdll::Updater::PeripheralConfig *config)
{
    CString strW;
    // TODO: Add your control notification handler code here
    if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
    {
        config->peripheralType = blfwkdll::Updater::HOST_PERIPHERAL_TYPE_TAG::kHostPeripheralType_UART;

        m_comboPortOrVID.GetWindowTextW(strW);
        CStringA strA(strW.GetBuffer());
        strW.ReleaseBuffer(); // Change UCHAR string to CHAR string by these three
                              // steps.
        config->comPortName = strA.GetBuffer();
        strA.ReleaseBuffer();

        m_comboBaudOrPID.GetWindowTextW(strW);
        int32_t speed;
        if (!cstringtoi(strW, speed))
        {
            AppendLog(_T("Error: Invalid Baudrate.(") + strW + _T(")"));
            return;
        }
        config->comPortSpeed = speed;
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_USBHID))->GetCheck() == BST_CHECKED)
    {
        uint32_t id;

        config->peripheralType = blfwkdll::Updater::HOST_PERIPHERAL_TYPE_TAG::kHostPeripheralType_USB_HID;

        m_comboPortOrVID.GetWindowTextW(strW);
        if (!cstringtoui(strW, id))
        {
            AppendLog(_T("Error: Invalid VID.(") + strW + _T(")"));
            return;
        }
        config->usbHidVid = id & 0xFFFF;

        m_comboBaudOrPID.GetWindowTextW(strW);
        if (!cstringtoui(strW, id))
        {
            AppendLog(_T("Error: Invalid PID.(") + strW + _T(")"));
            return;
        }
        config->usbHidPid = id & 0xFFFF;
    }
    config->ping = true;
    config->packetTimeoutMs = 5000;
}

void CKinetisFlashToolDlg::Connect(void)
{
    blfwkdll::Updater::PeripheralConfig config;
    GetPeripheralConfig(&config);
    g_pBootloader->Connect(&config);
    // SetTimer(1, 1000, NULL);
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::Reset(void)
{
    // KillTimer(1);
    g_pBootloader->Reset();
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnBnClickedRadio()
{
    // TODO: Add your control notification handler code here
    KillTimer(1); // Peripheral configuration is changed, stop the ping timer.
    if (g_pBootloader->m_isConneted)
    {
        Reset();
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
    {
        ((CEdit *)GetDlgItem(IDC_STATIC_PORT_VID))->SetWindowTextW(_T("Port:"));
        ((CEdit *)GetDlgItem(IDC_STATIC_BAUD_PID))->SetWindowTextW(_T("Baud:"));
        RefreshComPortList();
        RefreshBaudrateList();
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_USBHID))->GetCheck() == BST_CHECKED)
    {
        ((CEdit *)GetDlgItem(IDC_STATIC_PORT_VID))->SetWindowTextW(_T("VID:"));
        ((CEdit *)GetDlgItem(IDC_STATIC_BAUD_PID))->SetWindowTextW(_T("PID:"));
        RefreshPidList();
        RefreshVidList();
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnCbnSelchangeComboPortVid()
{
    // TODO: Add your control notification handler code here
    KillTimer(1); // Peripheral configuration is changed, stop the ping timer.
    if (g_pBootloader->m_isConneted)
    {
        Reset();
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
    {
        g_pHistoryData->SetCurrentDevice(m_comboPortOrVID.GetCurSel());
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_USBHID))->GetCheck() == BST_CHECKED)
    {
        g_pHistoryData->BubbleSelectedVid(m_comboPortOrVID.GetCurSel());
        RefreshVidList();
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnCbnSelchangeComboBaudPid()
{
    // TODO: Add your control notification handler code here
    KillTimer(1); // Peripheral configuration is changed, stop the ping timer.
    if (g_pBootloader->m_isConneted)
    {
        Reset();
    }

    if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
    {
        g_pHistoryData->SetCurrentBaudRate(m_comboBaudOrPID.GetCurSel());
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_USBHID))->GetCheck() == BST_CHECKED)
    {
        g_pHistoryData->BubbleSelectedPid(m_comboBaudOrPID.GetCurSel());
        RefreshVidList();
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnKillfocusComboPortVid()
{
    if (m_comboPortOrVID.GetCurSel() >= 0)
    {
        // If no change in the textbox, then return;
        return;
    }

    KillTimer(1); // Peripheral configuration is changed, stop the ping timer.
    if (g_pBootloader->m_isConneted)
    {
        Reset();
    }

    CString text;
    m_comboPortOrVID.GetWindowTextW(text);
    if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
    {
        // Find if there is the same COM port in the device list.
        for (size_t i = 0; i < g_pAllComDevices->GetDeviceCount(); i++)
        {
            if (text.CompareNoCase(g_pAllComDevices->GetDevice(i).GetString()) == 0)
            {
                // If yes, then set it as current selected.
                g_pHistoryData->SetCurrentDevice(i);
            }
        }
        RefreshComPortList();
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_USBHID))->GetCheck() == BST_CHECKED)
    {
        uint32_t intVid;
        if (!cstringtoui(text, intVid))
        {
            AppendLog(_T("Error: Invalid VID.(") + text + _T(")"));
        }
        else
        {
            g_pHistoryData->InsertVid(static_cast<uint16_t>(intVid));
        }
        RefreshVidList();
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnKillfocusComboBaudPid()
{
    if (m_comboBaudOrPID.GetCurSel() >= 0)
    {
        // If no change in the textbox, then return;
        return;
    }

    KillTimer(1); // Peripheral configuration is changed, stop the ping timer.
    if (g_pBootloader->m_isConneted)
    {
        Reset();
    }

    CString text;
    m_comboBaudOrPID.GetWindowTextW(text);
    if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
    {
        int32_t intBaud;
        if (!cstringtoi(text, intBaud))
        {
            AppendLog(_T("Error: Invalid Baud Rate.(") + text + _T(")"));
        }
        else
        {
            g_pHistoryData->InsertBaudRate(intBaud);
        }
        RefreshBaudrateList();
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_USBHID))->GetCheck() == BST_CHECKED)
    {
        uint32_t intPid;
        if (!cstringtoui(text, intPid))
        {
            AppendLog(_T("Error: Invalid PID.(") + text + _T(")"));
        }
        else
        {
            g_pHistoryData->InsertPid(static_cast<uint16_t>(intPid));
        }
        RefreshPidList();
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnClickedButtonCntReset()
{
    if (!g_pBootloader->m_isConneted)
    {
        Connect();
        SetTimer(1, 1000, NULL);
    }
    else
    {
        KillTimer(1);
        Reset();
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnTcnSelchangeTabFunc(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    *pResult = 0;
    switch (m_tabFunc.GetCurSel())
    {
        case 0:
            m_updateDlg.ShowWindow(true);
            m_flashDlg.ShowWindow(false);
            m_bootloaderConfigDLg.ShowWindow(false);
            break;
        case 1:
            m_updateDlg.ShowWindow(false);
            m_flashDlg.ShowWindow(true);
            m_bootloaderConfigDLg.ShowWindow(false);
            break;
        case 2:
            m_updateDlg.ShowWindow(false);
            m_flashDlg.ShowWindow(false);
            m_bootloaderConfigDLg.ShowWindow(true);
            break;
        /*
        case 3:
            m_updateDlg.ShowWindow(false);
            m_flashDlg.ShowWindow(false);
            m_bootloaderConfigDLg.ShowWindow(false);
            break;
        case 4:
            m_updateDlg.ShowWindow(false);
            m_flashDlg.ShowWindow(false);
            m_bootloaderConfigDLg.ShowWindow(false);
            break;
        */
        default:
            break;
    }
}

// See CKinetisFlashToolDlg.h for documentation of this method.
void CKinetisFlashToolDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your control notification handler code here

    g_pBootloader->Ping();

    CDialogEx::OnTimer(nIDEvent);
}

BOOL CKinetisFlashToolDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
#ifdef _DEBUG
    AppendLog(_T("Device Change Event(") + format_cstring(_T("nEventType = %#x"), nEventType) + _T(")"));
#endif

    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)dwData;
    switch (nEventType)
    {
        case DBT_DEVICEARRIVAL:
        case DBT_DEVICEREMOVECOMPLETE:
        {
#ifdef _DEBUG
            AppendLog(_T("Device Change Event(") + format_cstring(_T("devicetype = %#x"), pHdr->dbch_devicetype) +
                      _T(")"));
#endif
            if (pHdr->dbch_devicetype == DBT_DEVTYP_PORT)
            {
#ifdef _DEBUG
                AppendLog(_T("Device Change Event(") +
                          format_cstring(_T("dbcp_name = %s"), ((PDEV_BROADCAST_PORT)pHdr)->dbcp_name) + _T(")"));
#endif
                // Backup previous selected device.
                SerialDevice preDev;
                if (g_pHistoryData->GetCurrentDevice() >= 0)
                {
                    preDev = g_pAllComDevices->GetDevice(g_pHistoryData->GetCurrentDevice());
                }

                g_pAllComDevices->ScanSerialDevices(); // Rescan the serial com devices.

                // If currently no device is selected, select the new device.
                if ((g_pHistoryData->GetCurrentDevice() < 0) && (nEventType == DBT_DEVICEARRIVAL))
                {
                    g_pHistoryData->SetCurrentDevice(
                        g_pAllComDevices->FindSerialDevice(((PDEV_BROADCAST_PORT)pHdr)->dbcp_name));
                }
                else if (g_pHistoryData->GetCurrentDevice() >= 0)
                {
                    g_pHistoryData->SetCurrentDevice(g_pAllComDevices->FindSerialDevice(preDev));
                }
                if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
                {
                    RefreshComPortList();
                }
            }
            /* For OpenSDA CDC device, DBT_DEVTYP_PORT event will not be broadcasted.
             * Use DBT_DEVTYP_DEVICEINTERFACE event to handle the changes.
             */
            else if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
#ifdef _DEBUG
                AppendLog(_T("Device Change Event(") +
                          format_cstring(_T("dbcc_name = %s"), ((PDEV_BROADCAST_DEVICEINTERFACE)pHdr)->dbcc_name) +
                          _T(")"));
#endif
                CString devPath = ((PDEV_BROADCAST_DEVICEINTERFACE)pHdr)->dbcc_name;
                // Check whether the event is trigged by OpenSDA device.
                // Only handle OpenSDA device, others will be handled by DBT_DEVTYP_PORT.
                if (devPath.Find(format_cstring(_T("VID_%04x&PID_%04x"), 0x1357, 0x0089)) != -1)
                {
                    // Backup previous selected device.
                    SerialDevice preDev;
                    if (g_pHistoryData->GetCurrentDevice() >= 0)
                    {
                        preDev = g_pAllComDevices->GetDevice(g_pHistoryData->GetCurrentDevice());
                    }

                    Sleep(500);                            // Wait for Windows Registry refresh
                    g_pAllComDevices->ScanSerialDevices(); // Rescan the serial com devices.
                    // If currently no device is selected, select the new device
                    if ((g_pHistoryData->GetCurrentDevice() < 0) && (nEventType == DBT_DEVICEARRIVAL))
                    {
                        HDEVINFO devInfo = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_USB_DEVICE, NULL, 0,
                                                                DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

                        CString devId = devPath.Left(devPath.ReverseFind(_T('#')));
                        devId = devId.Right(devId.GetLength() - devId.Find(_T("USB")));
                        devId.Replace(_T('#'), _T('\\'));

                        SP_DEVINFO_DATA devInfoData;
                        ZeroMemory(&devInfoData, sizeof(SP_DEVINFO_DATA));
                        devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
                        SetupDiOpenDeviceInfo(devInfo, (PCTSTR)devId.GetString(), NULL,
                                              DIOD_INHERIT_CLASSDRVS | DIOD_CANCEL_REMOVE, &devInfoData);

                        // The first child node of OpenSDA is the COM device node.
                        DEVINST portDevInst;
                        if (CM_Get_Child(&portDevInst, devInfoData.DevInst, 0) != CR_SUCCESS)
                        {
                            goto Refresh;
                        }

                        // Get COM device node's info.
                        ULONG idSize;
                        CString portDevId;
                        CM_Get_Device_ID_Size(&idSize, portDevInst, 0);
                        CM_Get_Device_ID(portDevInst, portDevId.GetBufferSetLength(idSize), idSize, 0);
                        portDevId.ReleaseBuffer();

                        SP_DEVINFO_DATA portDevInfoData;
                        ZeroMemory(&portDevInfoData, sizeof(SP_DEVINFO_DATA));
                        portDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
                        SetupDiOpenDeviceInfo(devInfo, (PCTSTR)portDevId.GetString(), NULL,
                                              DIOD_INHERIT_CLASSDRVS | DIOD_CANCEL_REMOVE, &portDevInfoData);

                        if (portDevInfoData.ClassGuid == GUID_DEVCLASS_PORTS)
                        {
                            // Get the com port from the friendly name.
                            ULONG bufSize = 0;
                            SetupDiGetDeviceRegistryProperty(devInfo, &portDevInfoData, SPDRP_FRIENDLYNAME, NULL, NULL,
                                                             0, &bufSize);
                            BYTE *pBuffer = (BYTE *)malloc(bufSize);
                            SetupDiGetDeviceRegistryProperty(devInfo, &portDevInfoData, SPDRP_FRIENDLYNAME, NULL,
                                                             pBuffer, bufSize, &bufSize);

                            CString friendlyName = CString((LPCWSTR)pBuffer);
                            CString comPort = friendlyName.Mid(friendlyName.ReverseFind(_T('(')) + 1);
                            comPort.TrimRight(_T(')'));
                            if (pBuffer != NULL)
                            {
                                free(pBuffer);
                                pBuffer = NULL;
                            }
                            g_pHistoryData->SetCurrentDevice(g_pAllComDevices->FindSerialDevice(SerialDevice(comPort)));
                        }
                    }
                    else if (g_pHistoryData->GetCurrentDevice() >= 0)
                    {
                        g_pHistoryData->SetCurrentDevice(g_pAllComDevices->FindSerialDevice(preDev));
                    }
                Refresh:
                    if (((CButton *)GetDlgItem(IDC_RADIO_UART))->GetCheck() == BST_CHECKED)
                    {
                        RefreshComPortList();
                    }
                }
            }
#ifdef _DEBUG
            else if (pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                AppendLog(_T("Device Change Event(") +
                          format_cstring(_T("uintmask = %#x"), ((PDEV_BROADCAST_VOLUME)pHdr)->dbcv_unitmask) + _T(")"));
            }
#endif
            break;
        }
        default:
            break;
    }

    return CDialog::OnDeviceChange(nEventType, dwData);
}
// See CKinetisFlashToolDlg.h for documentation of this method.
LRESULT CKinetisFlashToolDlg::OnUpdateLog(WPARAM wParam, LPARAM lParam)
{
    LPWSTR log = (LPWSTR)wParam;
    AppendLog(log);
    return 0;
}

// See CKinetisFlashToolDlg.h for documentation of this method.
LRESULT CKinetisFlashToolDlg::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
    if (g_pBootloader->m_isConneted)
    {
        m_bootloaderVersion =
            format_cstring(_T("%c%d.%d.%d"), (g_pBootloader->m_targetInfo.targetVersion & 0xff000000) >> 24,
                           (g_pBootloader->m_targetInfo.targetVersion & 0x00ff0000) >> 16,
                           (g_pBootloader->m_targetInfo.targetVersion & 0x0000ff00) >> 8,
                           g_pBootloader->m_targetInfo.targetVersion & 0x000000ff);
        m_protoclVersion =
            format_cstring(_T("%c%d.%d.%d"), (g_pBootloader->m_targetInfo.procotolVersion & 0xff000000) >> 24,
                           (g_pBootloader->m_targetInfo.procotolVersion & 0x00ff0000) >> 16,
                           (g_pBootloader->m_targetInfo.procotolVersion & 0x0000ff00) >> 8,
                           g_pBootloader->m_targetInfo.procotolVersion & 0x000000ff);
        m_securityState =
            format_cstring(_T("%s"), g_pBootloader->m_targetInfo.securityState ? _T("SECURE") : _T("UNSECURE"));
        if (!g_pBootloader->m_targetInfo.flashSize)
        {
            m_flashSize = _T("-");
        }
        else
        {
            m_flashSize = format_cstring(_T("%s"), scale_bytes(g_pBootloader->m_targetInfo.flashSize));
        }

        m_ramSize = format_cstring(_T("%s"), scale_bytes(g_pBootloader->m_targetInfo.ramSize));

        m_rsvRgnFlashStart = format_cstring(_T("0x%X"), g_pBootloader->m_targetInfo.rsvRgnFlashStart);
        m_rsvRgnFlashEnd = format_cstring(_T("0x%X"), g_pBootloader->m_targetInfo.rsvRgnFlashEnd);
        m_rsvRgnRamStart = format_cstring(_T("0x%X"), g_pBootloader->m_targetInfo.rsvRgnRamStart);
        m_rsvRgnRamEnd = format_cstring(_T("0x%X"), g_pBootloader->m_targetInfo.rsvRgnRamEnd);

        AppendLog(_T("Device information is updated!"));
    }
    return 0;
}

// See CKinetisFlashToolDlg.h for documentation of this method.
LRESULT CKinetisFlashToolDlg::OnChangeConnection(WPARAM wParam, LPARAM lParam)
{
    if (g_pBootloader->m_isConneted)
    {
        AppendLog(_T("Connected to device successfully!"));
        ((CButton *)GetDlgItem(IDC_BUTTON_CNT_RESET))->SetWindowTextW(_T("Reset"));
        AppendLog(_T("Collecting device information......"));
        g_pBootloader->UpdateStatus();
    }
    else
    {
        m_bootloaderVersion.Empty();
        m_protoclVersion.Empty();
        m_securityState.Empty();
        m_flashSize.Empty();
        m_ramSize.Empty();
        m_rsvRgnFlashStart.Empty();
        m_rsvRgnFlashEnd.Empty();
        m_rsvRgnRamStart.Empty();
        m_rsvRgnRamEnd.Empty();

        AppendLog(_T("Disconnected from device!\r\n"));
        ((CButton *)GetDlgItem(IDC_BUTTON_CNT_RESET))->SetWindowTextW(_T("Connect"));
    }
    ::PostMessage(g_pMainDlg->m_updateDlg, USER_MS_CONNECTION_CHANGE, NULL, NULL);

    return 0;
}
