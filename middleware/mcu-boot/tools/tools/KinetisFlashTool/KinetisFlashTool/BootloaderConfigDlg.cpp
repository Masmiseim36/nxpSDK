/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include "KinetisFlashTool.h"
#include "BootloaderConfigDlg.h"
#include "afxdialogex.h"
#include "utility.h"
#include "BootloaderConfigText.h"

// BootloaderConfigDlg dialog
extern void PrintLog(const wchar_t *format, ...);

IMPLEMENT_DYNAMIC(BootloaderConfigDlg, CDialogEx)

// See BootloaderConfigDlg.h for documentation of this method.
BootloaderConfigDlg::BootloaderConfigDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG_BOOTLOADERCONFIG, pParent)
    , m_config(NULL)
    , m_cFile(NULL)
{
}

// See BootloaderConfigDlg.h for documentation of this method.
BootloaderConfigDlg::~BootloaderConfigDlg()
{
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(BootloaderConfigDlg, CDialogEx)
ON_BN_CLICKED(IDC_CHECK_CRC, &BootloaderConfigDlg::OnBnClickedCheckCrc)
ON_BN_CLICKED(IDC_CHECK_I2CSLAVE_ADDRESS, &BootloaderConfigDlg::OnBnClickedCheckI2cslaveAddress)
ON_BN_CLICKED(IDC_CHECK_TIMEOUT, &BootloaderConfigDlg::OnBnClickedCheckTimeout)
ON_BN_CLICKED(IDC_CHECK_VID, &BootloaderConfigDlg::OnBnClickedCheckVid)
ON_BN_CLICKED(IDC_CHECK_PID, &BootloaderConfigDlg::OnBnClickedCheckPid)
ON_BN_CLICKED(IDC_CHECK_USBSTRING_POINTER, &BootloaderConfigDlg::OnBnClickedCheckUsbstringPointer)
ON_BN_CLICKED(IDC_CHECK_ENABLE_HIGHSPEED, &BootloaderConfigDlg::OnBnClickedCheckDisableHighspeed)
ON_BN_CLICKED(IDC_CHECK_MMCAU_POINTER, &BootloaderConfigDlg::OnBnClickedCheckMmcauPointer)
ON_BN_CLICKED(IDC_CHECK_OTFAD_POINTER, &BootloaderConfigDlg::OnBnClickedCheckOtfadPointer)
ON_BN_CLICKED(IDC_CHECK_QSPI_POINTER, &BootloaderConfigDlg::OnBnClickedCheckQspiPointer)
ON_BN_CLICKED(IDC_RELOAD, &BootloaderConfigDlg::OnBnClickedReload)
ON_BN_CLICKED(IDC_BUTTON_CONFIG_OK, &BootloaderConfigDlg::OnBnClickedButtonConfigOk)
ON_BN_CLICKED(IDC_BUTTON_CONFIG_CANCEL, &BootloaderConfigDlg::OnBnClickedButtonConfigCancel)
ON_BN_CLICKED(IDC_CHECK_CANSPEED, &BootloaderConfigDlg::OnBnClickedCheckCanspeed)
ON_BN_CLICKED(IDC_RADIO_SPEEDINDEX1, &BootloaderConfigDlg::OnBnClickedRadioSpeedindex)
ON_BN_CLICKED(IDC_RADIO_SPEEDINDEX2, &BootloaderConfigDlg::OnBnClickedRadioSpeedindex)
ON_BN_CLICKED(IDC_RADIO_SPEEDINDEX3, &BootloaderConfigDlg::OnBnClickedRadioSpeedindex)
ON_BN_CLICKED(IDC_RADIO_SPEEDINDEX4, &BootloaderConfigDlg::OnBnClickedRadioSpeedindex)
ON_BN_CLICKED(IDC_RADIO_SPEEDINDEX5, &BootloaderConfigDlg::OnBnClickedRadioSpeedindex)
ON_BN_CLICKED(IDC_CHECK_CANTXID, &BootloaderConfigDlg::OnBnClickedCheckCantxid)
ON_BN_CLICKED(IDC_CHECK_CANRXID, &BootloaderConfigDlg::OnBnClickedCheckCanrxid)
ON_BN_CLICKED(IDC_BUTTON_CONFIG_GENERATE, &BootloaderConfigDlg::OnBnClickedButtonConfigGenerate)
END_MESSAGE_MAP()

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnOK()
{
    // CDialogEx::OnOK();
}

// See BootloaderConfigDlg.h for documentation of this method.
BOOL BootloaderConfigDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO: Add extra initialization here
    // Invisible for crc length input, length is always the file length.
    GetDlgItem(IDC_STATIC_CRC_LENGTH)->ShowWindow(false);
    GetDlgItem(IDC_EDIT_CRC_LENGTH)->ShowWindow(false);
    // Currently, CAN clock config is not supported.
    GetDlgItem(IDC_CHECK_CANCLK)->ShowWindow(false);
    GetDlgItem(IDC_RADIO_OSCERCLK)->ShowWindow(false);
    GetDlgItem(IDC_RADIO_BUSCLK)->ShowWindow(false);
    if (m_config)
    {
        LoadBootloaderConfigureArea();
    }

    return TRUE; // return TRUE unless you set the focus to a control
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::LoadBootloaderConfigureArea()
{
    // Tag
    if ((m_config->tag == kBootloaderConfigAreaTag) || (m_cFile == NULL))
        ((CButton *)GetDlgItem(IDC_CHECK_TAG))->SetCheck(true);
    else
        ((CButton *)GetDlgItem(IDC_CHECK_TAG))->SetCheck(false);

    // CRC
    if (m_cFile != NULL)
    {
        // If all of the three params are not all 1s, we consider the crc check is enabled.
        if ((m_config->crcStartAddress != 0xFFFFFFFF) && (m_config->crcByteCount != 0xFFFFFFFF) &&
            (m_config->crcExpectedValue != 0xFFFFFFFF))
        {
            ((CButton *)GetDlgItem(IDC_CHECK_CRC))->SetCheck(true);
            // Start address is not the start offset of the image,
            // It is the real start address where image to write to.
            GetDlgItem(IDC_STATIC_CRC_STARTADDRESS)->EnableWindow(true);
            GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)->EnableWindow(true);
            GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)
                ->SetWindowTextW(format_cstring(_T("0x%08X"), m_config->crcStartAddress));
        }
        else // disabled crc
        {
            ((CButton *)GetDlgItem(IDC_CHECK_CRC))->SetCheck(false);
            GetDlgItem(IDC_STATIC_CRC_STARTADDRESS)->EnableWindow(false);
            GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)->EnableWindow(false);
            GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)
                ->SetWindowTextW(format_cstring(_T("0x%08X"), 0x00000000)); // default 0
        }
    }
    else
    {
        // If file pointer is NULL, the disable CRC feature.
        // Because CRC calculation needs the finnal binaries.
        GetDlgItem(IDC_CHECK_CRC)->EnableWindow(false);
        ((CButton *)GetDlgItem(IDC_CHECK_CRC))->SetCheck(false);
        GetDlgItem(IDC_STATIC_CRC_STARTADDRESS)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)->SetWindowTextW(format_cstring(_T("0x%08X"), 0x00000000)); // default 0
    }

    // Peripheral config
    // "Checked" means the corresponding peripheral is enabled.
    // Uart
    if (m_config->enabledPeripherals & kPeripheralMaskUart)
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_UART))->SetCheck(true);
    else
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_UART))->SetCheck(false);
    // I2C
    if (m_config->enabledPeripherals & kPeripheralMaskI2c)
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_I2C))->SetCheck(true);
    else
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_I2C))->SetCheck(false);
    // SPI
    if (m_config->enabledPeripherals & kPeripheralMaskSpi)
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_SPI))->SetCheck(true);
    else
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_SPI))->SetCheck(false);
    // CAN
    if (m_config->enabledPeripherals & kPeripheralMaskCan)
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_CAN))->SetCheck(true);
    else
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_CAN))->SetCheck(false);
    // USB
    if (m_config->enabledPeripherals & kPeripheralMaskUsb)
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_USB))->SetCheck(true);
    else
        ((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_USB))->SetCheck(false);

    // I2c slave address
    if (m_config->i2cSlaveAddress != 0xFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_I2CSLAVE_ADDRESS))->SetCheck(true);
        GetDlgItem(IDC_EDIT_I2C_ADDRESS)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_I2C_ADDRESS)->SetWindowTextW(format_cstring(_T("0x%02X"), m_config->i2cSlaveAddress));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_I2CSLAVE_ADDRESS))->SetCheck(false);
        GetDlgItem(IDC_EDIT_I2C_ADDRESS)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_I2C_ADDRESS)->SetWindowTextW(format_cstring(_T("0x%02X"), 0x10)); // default 0x10
    }

    // Peripheral detection timeout
    if (m_config->peripheralDetectionTimeoutMs != 0xFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_TIMEOUT))->SetCheck(true);
        GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(true);
        GetDlgItem(IDC_STATIC_TIMEOUT_MS)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_TIMEOUT)->SetWindowTextW(format_cstring(_T("%hu"), m_config->peripheralDetectionTimeoutMs));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_TIMEOUT))->SetCheck(false);
        GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(false);
        GetDlgItem(IDC_STATIC_TIMEOUT_MS)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_TIMEOUT)->SetWindowTextW(format_cstring(_T("%hu"), 5000)); // default 5000ms
    }

    // USB config
    // VID
    if (m_config->usbVid != 0xFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_VID))->SetCheck(true);
        GetDlgItem(IDC_EDIT_USBVID)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_USBVID)->SetWindowTextW(format_cstring(_T("0x%04X"), m_config->usbVid));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_VID))->SetCheck(false);
        GetDlgItem(IDC_EDIT_USBVID)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_USBVID)->SetWindowTextW(format_cstring(_T("0x%04X"), 0x15A2)); // default 0x15A2
    }
    // PID
    if (m_config->usbPid != 0xFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_PID))->SetCheck(true);
        GetDlgItem(IDC_EDIT_USBPID)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_USBPID)->SetWindowTextW(format_cstring(_T("0x%04X"), m_config->usbPid));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_PID))->SetCheck(false);
        GetDlgItem(IDC_EDIT_USBPID)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_USBPID)->SetWindowTextW(format_cstring(_T("0x%04X"), 0x0073)); // default 0x0073
    }
    // USB string pointer
    if (m_config->usbStringsPointer != 0xFFFFFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_USBSTRING_POINTER))->SetCheck(true);
        GetDlgItem(IDC_EDIT_USBSTRING)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_USBSTRING)->SetWindowTextW(format_cstring(_T("0x%08X"), m_config->usbStringsPointer));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_USBSTRING_POINTER))->SetCheck(false);
        GetDlgItem(IDC_EDIT_USBSTRING)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_USBSTRING)->SetWindowTextW(format_cstring(_T("0x%08X"), 0x00000000));
    }

    // Clock
    if ((m_config->clockFlags & kClockFlagHighSpeed) == 0)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_ENABLE_HIGHSPEED))->SetCheck(true);
        GetDlgItem(IDC_STATIC_CLKDIV)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_CLOCKDIVIDER)->EnableWindow(true);
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_ENABLE_HIGHSPEED))->SetCheck(false);
        GetDlgItem(IDC_STATIC_CLKDIV)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_CLOCKDIVIDER)->EnableWindow(false);
    }
    // clockDivider is inverted value of the divider used for core and bus clocks when in highspeed mode.
    // It is invalid when highspeed mode is disabled.
    GetDlgItem(IDC_EDIT_CLOCKDIVIDER)->SetWindowTextW(format_cstring(_T("%hhu"), ~m_config->clockDivider));

    // Direct boot
    // If bit 0 is cleared, then Kinetis bootloader will jump to application depending on FOPT BOOTSRC_SEL bits.
    // If the bit is set, then Kinetis bootloader will prepare for host communication over supported peripherals.
    if ((~m_config->bootFlags) & kBootFlagDirectBoot)
        ((CButton *)GetDlgItem(IDC_CHECK_DIRECTBOOT))->SetCheck(true);
    else
        ((CButton *)GetDlgItem(IDC_CHECK_DIRECTBOOT))->SetCheck(false);

    // MMCAU lib pointer
    if (m_config->mmcauConfigPointer != 0xFFFFFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_MMCAU_POINTER))->SetCheck(true);
        GetDlgItem(IDC_EDIT_MMCAU)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_MMCAU)->SetWindowTextW(format_cstring(_T("0x%08X"), m_config->mmcauConfigPointer));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_MMCAU_POINTER))->SetCheck(false);
        GetDlgItem(IDC_EDIT_MMCAU)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_MMCAU)->SetWindowTextW(format_cstring(_T("0x%08X"), 0x00000000));
    }

    // Key blob pointer
    if (m_config->keyBlobPointer != 0xFFFFFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_OTFAD_POINTER))->SetCheck(true);
        GetDlgItem(IDC_EDIT_OTFAD)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_OTFAD)->SetWindowTextW(format_cstring(_T("0x%08X"), m_config->keyBlobPointer));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_OTFAD_POINTER))->SetCheck(false);
        GetDlgItem(IDC_EDIT_OTFAD)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_OTFAD)->SetWindowTextW(format_cstring(_T("0x%08X"), 0x00000000));
    }

    // CAN config
    // TXID
    if (m_config->canTxId != 0xFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_CANTXID))->SetCheck(true);
        GetDlgItem(IDC_EDIT_CANTXID)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_CANTXID)
            ->SetWindowTextW(format_cstring(_T("0x%03X"), m_config->canTxId & 0x7FF)); // Only supports 11bits ID
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_CANTXID))->SetCheck(false);
        GetDlgItem(IDC_EDIT_CANTXID)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_CANTXID)->SetWindowTextW(format_cstring(_T("0x%03X"), 0x123)); // default 0x123
    }
    // RXID
    if (m_config->canRxId != 0xFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_CANRXID))->SetCheck(true);
        GetDlgItem(IDC_EDIT_CANRXID)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_CANRXID)
            ->SetWindowTextW(format_cstring(_T("0x%03X"), m_config->canRxId & 0x7FF)); // Only supports 11bits ID
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_CANRXID))->SetCheck(false);
        GetDlgItem(IDC_EDIT_CANRXID)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_CANRXID)->SetWindowTextW(format_cstring(_T("0x%03X"), 0x321)); // default 0x321
    }
    // CAN Speed config
    bool displaySpecifiedConfig = false;
    if ((m_config->canConfig1 & FLEXCAN_SPEED_MASK) != 0x0F)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_CANSPEED))->SetCheck(true);
        GetDlgItem(IDC_RADIO_SPEEDINDEX1)->EnableWindow(true);
        GetDlgItem(IDC_RADIO_SPEEDINDEX2)->EnableWindow(true);
        GetDlgItem(IDC_RADIO_SPEEDINDEX3)->EnableWindow(true);
        GetDlgItem(IDC_RADIO_SPEEDINDEX4)->EnableWindow(true);
        GetDlgItem(IDC_RADIO_SPEEDINDEX5)->EnableWindow(true);
        if (m_config->canConfig1 & FLEXCAN_SPEED_SPEC_MASK) // Specified speed config
        {
            ((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX5))->SetCheck(true);
            displaySpecifiedConfig = true;
        }
        else // Build-in speed config
        {
            switch (m_config->canConfig1 & FLEXCAN_SPEED_INDEX_MASK)
            {
                case 0:
                    ((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX1))->SetCheck(true);
                    break;
                case 1:
                    ((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX2))->SetCheck(true);
                    break;
                case 2:
                    ((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX3))->SetCheck(true);
                    break;
                // case 3:   // 750KHz will not be supported any more.
                //    break;
                case 4:
                    ((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX4))->SetCheck(true);
                default:
                    break;
            }
            displaySpecifiedConfig = false;
        }
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_CANSPEED))->SetCheck(false);
        GetDlgItem(IDC_RADIO_SPEEDINDEX1)->EnableWindow(false);
        GetDlgItem(IDC_RADIO_SPEEDINDEX2)->EnableWindow(false);
        GetDlgItem(IDC_RADIO_SPEEDINDEX3)->EnableWindow(false);
        GetDlgItem(IDC_RADIO_SPEEDINDEX4)->EnableWindow(false);
        GetDlgItem(IDC_RADIO_SPEEDINDEX5)->EnableWindow(false);
        displaySpecifiedConfig = false;
    }

    GetDlgItem(IDC_STATIC_PREDIV)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_EDIT_PREDIV)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_STATIC_PSEG1)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_EDIT_PSEG1)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_STATIC_PSEG2)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_EDIT_PSEG2)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_STATIC_RJW)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_EDIT_RJW)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_STATIC_PROPSEG)->EnableWindow(displaySpecifiedConfig);
    GetDlgItem(IDC_EDIT_PROPSEG)->EnableWindow(displaySpecifiedConfig);

    GetDlgItem(IDC_EDIT_PREDIV)
        ->SetWindowTextW(
            format_cstring(_T("0x%02X"), (m_config->canConfig2 & FLEXCAN_PRESCALER_MASK) >> FLEXCAN_PRESCALER_SHIFT));
    GetDlgItem(IDC_EDIT_PSEG1)
        ->SetWindowTextW(
            format_cstring(_T("0x%01X"), (m_config->canConfig2 & FLEXCAN_PSEG1_MASK) >> FLEXCAN_PSEG1_SHIFT));
    GetDlgItem(IDC_EDIT_PSEG2)
        ->SetWindowTextW(
            format_cstring(_T("0x%01X"), (m_config->canConfig2 & FLEXCAN_PSEG2_MASK) >> FLEXCAN_PSEG2_SHIFT));
    GetDlgItem(IDC_EDIT_RJW)->SetWindowTextW(format_cstring(_T("0x%01X"), m_config->canConfig2 & FLEXCAN_RJW_MASK));
    GetDlgItem(IDC_EDIT_PROPSEG)
        ->SetWindowTextW(
            format_cstring(_T("0x%01X"), (m_config->canConfig1 & FLEXCAN_PROPSEG_MASK) >> FLEXCAN_PROPSEG_SHIFT));

    // QSPI pointer
    if (m_config->qspi_config_block_pointer != 0xFFFFFFFF)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_QSPI_POINTER))->SetCheck(true);
        GetDlgItem(IDC_EDIT_QSPI)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_QSPI)->SetWindowTextW(format_cstring(_T("0x%08X"), m_config->qspi_config_block_pointer));
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_QSPI_POINTER))->SetCheck(false);
        GetDlgItem(IDC_EDIT_QSPI)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_QSPI)->SetWindowTextW(format_cstring(_T("0x%08X"), 0x00000000));
    }
}

// See BootloaderConfigDlg.h for documentation of this method.
int BootloaderConfigDlg::SaveBootloaderConfigureArea()
{
    CString param;
    uint32_t value;
    size_t length = 0;
    char *fileData = NULL;
    blfwkdll::BootloaderConfigureAreaData *bcaData = NULL;

    if (m_cFile != NULL)
    {
        // If binary file is specified, read the whole file for CRC calculation.
        length = (size_t)m_cFile->GetLength();
        fileData = (char *)malloc(length);
        m_cFile->SeekToBegin(); // Set file position pointer to beginning.
        m_cFile->Read(fileData, length);
        // Malloc a memory region to save the configuration data.
        // Should not modified m_config directly, for the reason that there might be error during the flowing progress.
        bcaData = (blfwkdll::BootloaderConfigureAreaData *)(fileData + 0x3C0);
    }
    else
    {
        // If binary file is not specified, malloc a temp memory region for the configuration.
        length = sizeof blfwkdll::BootloaderConfigureAreaData;
        fileData = (char *)malloc(length);
        // Should not modified m_config directly, for the reason that there might be error during the flowing progress.
        bcaData = (blfwkdll::BootloaderConfigureAreaData *)(fileData);
    }
    // Reset configuration data to all 0xFFs
    FillMemory(bcaData, sizeof blfwkdll::BootloaderConfigureAreaData, 0xFF);

    // Tag
    if (((CButton *)GetDlgItem(IDC_CHECK_TAG))->GetCheck() == BST_CHECKED)
    {
        bcaData->tag = (uint32_t)kBootloaderConfigAreaTag;
    }
    // If Tag is not checked, show a MessageBox to let user to do the determination.
    else if (MessageBox(_T("Configuration will not take effect, because \"Tag\" is not checked.\r\nPress \"OK\" to ")
                        _T("continue, \"Cancel\" for going back."),
                        _T("Warning"), MB_OKCANCEL) == IDCANCEL)
    {
        // If Cancle, go back to config.
        free(fileData);
        return IDCANCEL;
    }
    else
    {
        // If OK, Write 0xFFs to tag region.
        bcaData->tag = 0xFFFFFFFF;
    }

    // CRC
    if ((m_cFile != NULL) && (((CButton *)GetDlgItem(IDC_CHECK_CRC))->GetCheck() == BST_CHECKED))
    {
        GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid Image Address(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->crcStartAddress = value;
        bcaData->crcByteCount = length;
    }
    else
    {
        bcaData->crcStartAddress = 0xFFFFFFFF;
        bcaData->crcByteCount = 0xFFFFFFFF;
    }

    // Peripherals
    // UART
    if (((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_UART))->GetCheck() == BST_CHECKED)
    {
        bcaData->enabledPeripherals |= kPeripheralMaskUart;
    }
    else
    {
        bcaData->enabledPeripherals &= ~kPeripheralMaskUart;
    }
    // I2C
    if (((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_I2C))->GetCheck() == BST_CHECKED)
    {
        bcaData->enabledPeripherals |= kPeripheralMaskI2c;
    }
    else
    {
        bcaData->enabledPeripherals &= ~kPeripheralMaskI2c;
    }
    // SPI
    if (((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_SPI))->GetCheck() == BST_CHECKED)
    {
        bcaData->enabledPeripherals |= kPeripheralMaskSpi;
    }
    else
    {
        bcaData->enabledPeripherals &= ~kPeripheralMaskSpi;
    }
    // CAN
    if (((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_CAN))->GetCheck() == BST_CHECKED)
    {
        bcaData->enabledPeripherals |= kPeripheralMaskCan;
    }
    else
    {
        bcaData->enabledPeripherals &= ~kPeripheralMaskCan;
    }
    // USB
    if (((CButton *)GetDlgItem(IDC_CHECK_PERIPHERAL_USB))->GetCheck() == BST_CHECKED)
    {
        bcaData->enabledPeripherals |= kPeripheralMaskUsb;
    }
    else
    {
        bcaData->enabledPeripherals &= ~kPeripheralMaskUsb;
    }

    // I2C slave address
    if (((CButton *)GetDlgItem(IDC_CHECK_I2CSLAVE_ADDRESS))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_I2C_ADDRESS)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid I2C Address(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->i2cSlaveAddress = value & 0xFF;
    }
    else
    {
        bcaData->i2cSlaveAddress = 0xFF;
    }

    // Peripheral timeout
    if (((CButton *)GetDlgItem(IDC_CHECK_TIMEOUT))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_TIMEOUT)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid Timeout Value(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->peripheralDetectionTimeoutMs = value & 0xFFFF;
    }
    else
    {
        bcaData->peripheralDetectionTimeoutMs = 0xFFFF;
    }

    // USB
    // VID
    if (((CButton *)GetDlgItem(IDC_CHECK_VID))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_USBVID)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid VID(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->usbVid = value & 0xFFFF;
    }
    else
    {
        bcaData->usbVid = 0xFFFF;
    }
    // PID
    if (((CButton *)GetDlgItem(IDC_CHECK_PID))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_USBPID)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid PID(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->usbPid = value & 0xFFFF;
    }
    else
    {
        bcaData->usbPid = 0xFFFF;
    }
    // USB string pointer
    if (((CButton *)GetDlgItem(IDC_CHECK_USBSTRING_POINTER))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_USBSTRING)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid USB String Pointer(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->usbStringsPointer = value;
    }
    else
    {
        bcaData->usbStringsPointer = 0xFFFFFFFF;
    }

    // Clock
    if (((CButton *)GetDlgItem(IDC_CHECK_ENABLE_HIGHSPEED))->GetCheck() == BST_CHECKED)
    {
        // "Cheched" means enable high speed mode.
        // High speed flag bit is cleared, meaning enabled.
        bcaData->clockFlags &= ~kClockFlagHighSpeed;

        // clockDivider is only valid at high speed mode.
        GetDlgItem(IDC_EDIT_CLOCKDIVIDER)->GetWindowTextW(param);
        // clockDivider is inverted value of the real divider used for bootloader.
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid Clock Divider(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->clockDivider = (~value) & 0xFF;
    }
    else
    {
        bcaData->clockFlags |= kClockFlagHighSpeed;
        bcaData->clockDivider = 0xFF;
    }

    // BootFlags
    if (((CButton *)GetDlgItem(IDC_CHECK_DIRECTBOOT))->GetCheck() == BST_CHECKED)
    {
        // "checked" means direct boot.
        // The first bit of bootFlags( bit[0]) should be cleared(0) to enable direct boot
        bcaData->bootFlags &= ~kBootFlagDirectBoot;
    }
    else
    {
        bcaData->bootFlags |= kBootFlagDirectBoot;
    }

    // MMCAU config pointer
    if (((CButton *)GetDlgItem(IDC_CHECK_MMCAU_POINTER))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_MMCAU)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid MMCAU Lib Pointer(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->mmcauConfigPointer = value;
    }
    else
    {
        bcaData->mmcauConfigPointer = 0xFFFFFFFF;
    }

    // OTFAD
    if (((CButton *)GetDlgItem(IDC_CHECK_OTFAD_POINTER))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_OTFAD)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid Key Blob Pointer(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->keyBlobPointer = value;
    }
    else
    {
        bcaData->keyBlobPointer = 0xFFFFFFFF;
    }

    // CAN config
    // TXID
    if (((CButton *)GetDlgItem(IDC_CHECK_CANTXID))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_CANTXID)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid CAN TXID(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->canTxId = value & 0x7FF;
    }
    else
    {
        bcaData->canTxId = 0xFFFF;
    }
    // RXID
    if (((CButton *)GetDlgItem(IDC_CHECK_CANRXID))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_CANRXID)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid CAN RXID(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->canRxId = value & 0x7FF;
    }
    else
    {
        bcaData->canRxId = 0xFFFF;
    }
    // CAN speed config
    if (((CButton *)GetDlgItem(IDC_CHECK_CANSPEED))->GetCheck() == BST_CHECKED)
    {
        if (((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX1))->GetCheck() == BST_CHECKED)
        {
            bcaData->canConfig1 = (bcaData->canConfig1 & (~FLEXCAN_SPEED_MASK)) | FLEXCAN_SPEED_125K;
        }
        else if (((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX2))->GetCheck() == BST_CHECKED)
        {
            bcaData->canConfig1 = (bcaData->canConfig1 & (~FLEXCAN_SPEED_MASK)) | FLEXCAN_SPEED_250K;
        }
        else if (((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX3))->GetCheck() == BST_CHECKED)
        {
            bcaData->canConfig1 = (bcaData->canConfig1 & (~FLEXCAN_SPEED_MASK)) | FLEXCAN_SPEED_500K;
        }
        else if (((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX4))->GetCheck() == BST_CHECKED)
        {
            bcaData->canConfig1 = (bcaData->canConfig1 & (~FLEXCAN_SPEED_MASK)) | FLEXCAN_SPEED_1M;
        }
        else if (((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX5))->GetCheck() == BST_CHECKED)
        {
            bcaData->canConfig1 |= FLEXCAN_SPEED_SPEC_MASK;

            GetDlgItem(IDC_EDIT_PREDIV)->GetWindowTextW(param);
            if (!cstringtoui(param, value))
            {
                MessageBox(_T("Error: Invalid PREVID Value(") + param + _T(")"), _T("Error"));
                free(fileData);
                return IDCANCEL;
            }
            bcaData->canConfig2 =
                (bcaData->canConfig2 & (~FLEXCAN_PRESCALER_MASK)) | ((value & 0xFF) << FLEXCAN_PRESCALER_SHIFT);
            GetDlgItem(IDC_EDIT_PSEG1)->GetWindowTextW(param);
            if (!cstringtoui(param, value))
            {
                MessageBox(_T("Error: Invalid PSEG1 Value(") + param + _T(")"), _T("Error"));
                free(fileData);
                return IDCANCEL;
            }
            bcaData->canConfig2 =
                (bcaData->canConfig2 & (~FLEXCAN_PSEG1_MASK)) | ((value & 0x7) << FLEXCAN_PSEG1_SHIFT);
            GetDlgItem(IDC_EDIT_PSEG2)->GetWindowTextW(param);
            if (!cstringtoui(param, value))
            {
                MessageBox(_T("Error: Invalid PSEG2 Value(") + param + _T(")"), _T("Error"));
                free(fileData);
                return IDCANCEL;
            }
            bcaData->canConfig2 =
                (bcaData->canConfig2 & (~FLEXCAN_PSEG2_MASK)) | ((value & 0x7) << FLEXCAN_PSEG2_SHIFT);
            GetDlgItem(IDC_EDIT_RJW)->GetWindowTextW(param);
            if (!cstringtoui(param, value))
            {
                MessageBox(_T("Error: Invalid RJW Value(") + param + _T(")"), _T("Error"));
                free(fileData);
                return IDCANCEL;
            }
            bcaData->canConfig2 = (bcaData->canConfig2 & (~FLEXCAN_RJW_MASK)) | ((value & 0x3) << FLEXCAN_RJW_SHIFT);
            GetDlgItem(IDC_EDIT_PROPSEG)->GetWindowTextW(param);
            if (!cstringtoui(param, value))
            {
                MessageBox(_T("Error: Invalid PROPSEG Value(") + param + _T(")"), _T("Error"));
                free(fileData);
                return IDCANCEL;
            }
            bcaData->canConfig1 =
                (bcaData->canConfig1 & (~FLEXCAN_PROPSEG_MASK)) | ((value & 0x7) << FLEXCAN_PROPSEG_SHIFT);
        }
    }
    else
    {
        bcaData->canConfig1 |= FLEXCAN_SPEED_MASK;
    }

    // QSPI
    if (((CButton *)GetDlgItem(IDC_CHECK_QSPI_POINTER))->GetCheck() == BST_CHECKED)
    {
        GetDlgItem(IDC_EDIT_QSPI)->GetWindowTextW(param);
        if (!cstringtoui(param, value))
        {
            MessageBox(_T("Error: Invalid QSPI Config Pointer(") + param + _T(")"), _T("Error"));
            free(fileData);
            return IDCANCEL;
        }
        bcaData->qspi_config_block_pointer = value;
    }
    else
    {
        bcaData->qspi_config_block_pointer = 0xFFFFFFFF;
    }

    // If enable crc check, then calculate the expected value.
    if ((m_cFile != NULL) && (((CButton *)GetDlgItem(IDC_CHECK_CRC))->GetCheck() == BST_CHECKED))
    {
        bcaData->crcExpectedValue = blfwkdll::Crc::calculate_application_crc32((uint8_t *)fileData, length);
    }
    else
    {
        bcaData->crcExpectedValue = 0xFFFFFFFF;
    }

    // Copy final BCA data to m_config;
    CopyMemory(m_config, bcaData, sizeof(blfwkdll::BootloaderConfigureAreaData));

    free(fileData);
    return IDOK;
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckCrc()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_CRC))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_STATIC_CRC_STARTADDRESS)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_CRC_STARTADDRESS)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckI2cslaveAddress()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_I2CSLAVE_ADDRESS))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_I2C_ADDRESS)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckTimeout()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_TIMEOUT))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_TIMEOUT_MS)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckVid()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_VID))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_USBVID)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckPid()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_PID))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_USBPID)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckUsbstringPointer()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_USBSTRING_POINTER))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_USBSTRING)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckDisableHighspeed()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_ENABLE_HIGHSPEED))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_STATIC_CLKDIV)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_CLOCKDIVIDER)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckMmcauPointer()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_MMCAU_POINTER))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_MMCAU)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckOtfadPointer()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_OTFAD_POINTER))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_OTFAD)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckQspiPointer()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_QSPI_POINTER))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_QSPI)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckCantxid()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_CANTXID))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_CANTXID)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckCanrxid()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_CANRXID))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_EDIT_CANRXID)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedCheckCanspeed()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_CHECK_CANSPEED))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_RADIO_SPEEDINDEX1)->EnableWindow(isChecked);
    GetDlgItem(IDC_RADIO_SPEEDINDEX2)->EnableWindow(isChecked);
    GetDlgItem(IDC_RADIO_SPEEDINDEX3)->EnableWindow(isChecked);
    GetDlgItem(IDC_RADIO_SPEEDINDEX4)->EnableWindow(isChecked);
    GetDlgItem(IDC_RADIO_SPEEDINDEX5)->EnableWindow(isChecked);
    if (isChecked)
    {
        isChecked = (((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX5))->GetCheck() == BST_CHECKED);
    }
    GetDlgItem(IDC_STATIC_PREDIV)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PREDIV)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_PSEG1)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PSEG1)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_PSEG2)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PSEG2)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_RJW)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_RJW)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_PROPSEG)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PROPSEG)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedRadioSpeedindex()
{
    bool isChecked = (((CButton *)GetDlgItem(IDC_RADIO_SPEEDINDEX5))->GetCheck() == BST_CHECKED);
    GetDlgItem(IDC_STATIC_PREDIV)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PREDIV)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_PSEG1)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PSEG1)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_PSEG2)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PSEG2)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_RJW)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_RJW)->EnableWindow(isChecked);
    GetDlgItem(IDC_STATIC_PROPSEG)->EnableWindow(isChecked);
    GetDlgItem(IDC_EDIT_PROPSEG)->EnableWindow(isChecked);
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedReload()
{
    LoadBootloaderConfigureArea();
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedButtonConfigOk()
{
    if (SaveBootloaderConfigureArea() == IDOK)
    {
        CDialogEx::OnOK();
    }
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedButtonConfigCancel()
{
    CDialogEx::OnCancel();
}

// See BootloaderConfigDlg.h for documentation of this method.
void BootloaderConfigDlg::OnBnClickedButtonConfigGenerate()
{
    // Save configuration to m_config first.
    if (SaveBootloaderConfigureArea() != IDOK)
    {
        return;
    }

    CFileDialog cFileDlg(FALSE, _T(".c"), _T("bootloader_config"),
                         OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR);

    cFileDlg.m_ofn.lpstrTitle = _T("Save Configuration File");
    cFileDlg.m_ofn.lpstrFilter = _T("C files(*.c)\0*.c;\0All files(*.*)\0*.*\0\0");
    if (IDOK != cFileDlg.DoModal())
    {
        // Save is canceled, stop save process.
        return;
    }

    CString filePath = cFileDlg.GetPathName();
    CFile cFile(filePath.GetBuffer(), CFile::modeWrite | CFile::modeCreate | CFile::typeBinary);

    // Write code text
    cFile.Write(code_section1, sizeof code_section1 - 1); // Do not write terminator "\0".

    CStringA configData, configText;
    configText.Empty();
    // Tag
    // Do not use Tab instead of space. On different editor, Tab has different width.
    configData = format_cstring(_T("\n        .tag = 0x%08X, //!< %s"), m_config->tag,
                                m_config->tag == kBootloaderConfigAreaTag ?
                                    _T("Magic Number") :
                                    _T("Invalid Magic Number. Configuration will not work."));
    configText.Append(configData);

    // CRC config cannot be generated without finnal binary image.
    if (((CButton *)GetDlgItem(IDC_CHECK_CRC))->GetCheck() == BST_CHECKED)
    {
        PrintLog(
            _T("Warning: CRC is disabled automatically, because CRC configuration won't work without final binary ")
            _T("image."));
    }
    configData = format_cstring(_T("\n        .crcStartAddress = 0xFFFFFFFF, //!< Disable CRC check"));
    configText.Append(configData);
    configData = format_cstring(_T("\n        .crcByteCount = 0xFFFFFFFF, //!< Disable CRC check"));
    configText.Append(configData);
    configData = format_cstring(_T("\n        .crcExpectedValue = 0xFFFFFFFF, //!< Disable CRC check"));
    configText.Append(configData);

    // Peripherals
    configData = format_cstring(_T("\n        .enabledPeripherals = 0x%02X, //!< Enabled Peripheral: %s%s%s%s%s%s"),
                                m_config->enabledPeripherals,
                                m_config->enabledPeripherals & kPeripheralMaskUart ? _T("UART ") : _T(""),
                                m_config->enabledPeripherals & kPeripheralMaskI2c ? _T("I2C ") : _T(""),
                                m_config->enabledPeripherals & kPeripheralMaskSpi ? _T("SPI ") : _T(""),
                                m_config->enabledPeripherals & kPeripheralMaskCan ? _T("CAN ") : _T(""),
                                m_config->enabledPeripherals & kPeripheralMaskUsb ? _T("USB-HID ") : _T(""),
                                m_config->enabledPeripherals & kPeripheralMask ? _T("") : _T("NONE"));
    configText.Append(configData);

    // I2C address
    configData = format_cstring(
        _T("\n        .i2cSlaveAddress = 0x%02X, //!< %s"), m_config->i2cSlaveAddress,
        m_config->i2cSlaveAddress != 0xFF ? _T("Use user-defined I2C address") : _T("Use default I2C address(0x10)"));
    configText.Append(configData);

    // Timeout
    configData = format_cstring(_T("\n        .peripheralDetectionTimeoutMs = 0x%04X, //!< %s"),
                                m_config->peripheralDetectionTimeoutMs,
                                m_config->peripheralDetectionTimeoutMs != 0xFFFF ? _T("Use user-defined timeout(ms)") :
                                                                                   _T("Use default timeout(5000ms)"));
    configText.Append(configData);

    // VID
    configData = format_cstring(
        _T("\n        .usbVid = 0x%04X, //!< %s"), m_config->usbVid,
        m_config->usbVid != 0xFFFF ? _T("Use user-defined Vendor ID") : _T("Use default Vendor ID(0x15A2)"));
    configText.Append(configData);

    // PID
    configData = format_cstring(
        _T("\n        .usbPid = 0x%04X, //!< %s"), m_config->usbPid,
        m_config->usbPid != 0xFFFF ? _T("Use user-defined Product ID") : _T("Use default Product ID(0x0073)"));
    configText.Append(configData);

    // USB string
    configData = format_cstring(
        _T("\n        .usbStringsPointer = 0x%08X, //!< %s"), m_config->usbStringsPointer,
        m_config->usbStringsPointer != 0xFFFFFFFF ? _T("Use user-defined USB String") : _T("Use default USB String"));
    configText.Append(configData);

    // clockFlags
    configData = format_cstring(
        _T("\n        .clockFlags = 0x%02X, //!< %s"), m_config->clockFlags,
        m_config->clockFlags & kClockFlagHighSpeed ? _T("Disable High speed mode") : _T("Enable High speed mode"));
    configText.Append(configData);

    // clockDivider
    configData = format_cstring(_T("\n        .clockDivider = 0x%02X, //!< Use clock divider(%hhu)"),
                                m_config->clockDivider, ~m_config->clockDivider);
    configText.Append(configData);

    // bootFlags
    configData = format_cstring(
        _T("\n        .bootFlags = 0x%02X, //!< %s"), m_config->bootFlags,
        m_config->bootFlags & kBootFlagDirectBoot ? _T("Enable communication with host") : _T("Enable direct boot"));
    configText.Append(configData);

    // MMCAU pointer
    configData = format_cstring(_T("\n        .mmcauConfigPointer = 0x%08X, //!< %s"), m_config->mmcauConfigPointer,
                                m_config->mmcauConfigPointer != 0xFFFFFFFF ? _T("MMCAU configuration is available") :
                                                                             _T("No MMCAU configuration"));
    configText.Append(configData);

    // OTFAD pointer
    configData =
        format_cstring(_T("\n        .keyBlobPointer = 0x%08X, //!< %s"), m_config->keyBlobPointer,
                       m_config->keyBlobPointer != 0xFFFFFFFF ? _T("Key blob is available") : _T("No key blob"));
    configText.Append(configData);

    // CAN config1
    configData =
        format_cstring(_T("\n        .canConfig1 = 0x%02X, //!< %s"), m_config->canConfig1,
                       m_config->canConfig1 != 0xFF ? _T("Use user-defined canConfig1") : _T("Use default canConfig1"));
    configText.Append(configData);

    // CAN config2
    configData = format_cstring(
        _T("\n        .canConfig2 = 0x%04X, //!< %s"), m_config->canConfig2,
        m_config->canConfig2 != 0xFFFF ? _T("Use user-defined canConfig2") : _T("Use default canConfig2"));
    configText.Append(configData);

    // CAN TX ID
    configData = format_cstring(
        _T("\n        .canTxId = 0x%04X, //!< %s"), m_config->canTxId,
        m_config->canTxId != 0xFFFF ? _T("Use user-defined CAN TX ID") : _T("Use default CAN TX ID(0x123)"));
    configText.Append(configData);

    // CAN RX ID
    configData = format_cstring(
        _T("\n        .canRxId = 0x%04X, //!< %s"), m_config->canRxId,
        m_config->canRxId != 0xFFFF ? _T("Use user-defined CAN RX ID") : _T("Use default CAN RX ID(0x321)"));
    configText.Append(configData);

    // QSPI pointer
    configData = format_cstring(_T("\n        .qspi_config_block_pointer = 0x%08X, //!< %s"),
                                m_config->qspi_config_block_pointer, m_config->qspi_config_block_pointer != 0xFFFFFFFF ?
                                                                         _T("QSPI configuration is available") :
                                                                         _T("No QSPI configuration "));
    configText.Append(configData);

    // Write configuration code text
    cFile.Write(configText.GetBuffer(), configText.GetLength());
    configText.ReleaseBuffer();

    // Write code text
    cFile.Write(code_section2, sizeof code_section2 - 1); // Do not write terminator "\0".
    cFile.Close();

    PrintLog(_T("Bootloader configuration file(%s) is generated successfully!"), filePath);
    CDialogEx::OnOK();
}
