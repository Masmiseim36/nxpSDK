/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "stdafx.h"
#include "KinetisFlashTool.h"
#include "KinetisFlashToolDlg.h"
#include "Bootloader.h"

extern CKinetisFlashToolDlg *g_pMainDlg;
extern void PrintLog(const wchar_t *format, ...);

UINT ThreadFunc(LPVOID pParam);

// See Bootloader.h for documentation of this method.
Bootloader *g_pBootloader;

// See Bootloader.h for documentation of this method.
DWORD Bootloader::Open()
{
    m_pingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_pingEvent == NULL)
        return GetLastError();

    m_statusEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_statusEvent == NULL)
        return GetLastError();

    m_resetEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_resetEvent == NULL)
        return GetLastError();

    m_connectEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_connectEvent == NULL)
        return GetLastError();

    m_updateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_updateEvent == NULL)
        return GetLastError();

    m_eraseEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_eraseEvent == NULL)
        return false;

    m_programOnceEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_programOnceEvent == NULL)
        return false;

    // Auto reset for exit event
    m_exitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_exitEvent == NULL)
        return false;

    m_isRunning = false;
    m_isExiting = false;

    m_updater = NULL;
    m_isConneted = false;
    m_config.ping = true;
    m_config.packetTimeoutMs = 5000;
    m_hasInternalFlash = false;
    m_pThread = AfxBeginThread(ThreadFunc, this, THREAD_PRIORITY_NORMAL, 0);

    return true;
}

// See Bootloader.h for documentation of this method.
void Bootloader::Close()
{
    if (m_isConneted)
    {
        while (!CanRun())
            ;
        SetEvent(m_resetEvent);
    }

    SetEvent(m_exitEvent);
    WaitForSingleObject(m_pThread->m_hThread, INFINITE);

    if (m_updater != NULL)
    {
        delete m_updater;
        m_updater = NULL;
    }

    CloseHandle(m_exitEvent);
    m_exitEvent = NULL;
    CloseHandle(m_programOnceEvent);
    m_programOnceEvent = NULL;
    CloseHandle(m_eraseEvent);
    m_eraseEvent = NULL;
    CloseHandle(m_pingEvent);
    m_pingEvent = NULL;
    CloseHandle(m_statusEvent);
    m_statusEvent = NULL;
    CloseHandle(m_resetEvent);
    m_resetEvent = NULL;
    CloseHandle(m_connectEvent);
    m_connectEvent = NULL;
    CloseHandle(m_updateEvent);
    m_updateEvent = NULL;
}

// See Bootloader.h for documentation of this method.
void Bootloader::progressCallback(int percentage, int segmentIndex, int segmentCount)
{
    // param segmentIndex starts from 1.
    // param percentage shows the percentage for current segment.
    // So need to calculate the finnal percentage of the progress here.
    int finalPercentage = (segmentIndex - 1) * 100 / segmentCount + percentage / segmentCount;
    ::SendMessage(g_pMainDlg->m_updateDlg, USER_MS_UPDATA_PROGRESS, (WPARAM)finalPercentage, NULL);
}

// See Bootloader.h for documentation of this method.
bool Bootloader::CanRun()
{
    if (m_pThread && !m_isRunning)
        return true;
    else
        return false;
}

// See Bootloader.h for documentation of this method.
void Bootloader::Ping()
{
    DWORD state = WaitForEvents(0);
#ifdef _DEBUG
    PrintLog(_T("Events state = %d at Ping()"), state);
#endif
    // Set this event only when no event is triggered.
    if (state == WAIT_TIMEOUT)
    {
        SetEvent(m_pingEvent);
    }
}

// See Bootloader.h for documentation of this method.
void Bootloader::UpdateStatus()
{
    if (!m_isConneted)
    {
        return;
    }
    // This event must always be triggered when connection is setup.
    // So no matter whether there is other event triggered, still trigger this
    // event.
    SetEvent(m_statusEvent);
}

// See Bootloader.h for documentation of this method.
void Bootloader::Reset()
{
    if (!m_isConneted)
    {
        return;
    }

    DWORD state = WaitForEvents(0);
#ifdef _DEBUG
    PrintLog(_T("Events state = %d at Reset()"), state);
#endif
    // if no event is triggered, or only ping event is triggered, set this event.
    if ((state == WAIT_OBJECT_0) || (state == WAIT_TIMEOUT))
    {
        SetEvent(m_resetEvent);
    }
}

// See Bootloader.h for documentation of this method.
void Bootloader::Connect(blfwkdll::Updater::PeripheralConfig *config)
{
    DWORD state = WaitForEvents(0);
#ifdef _DEBUG
    PrintLog(_T("Events state = %d at Connect()"), state);
#endif
    // if no event is triggered, or only ping event is triggered, set this event.
    if ((state == WAIT_OBJECT_0) || (state == WAIT_TIMEOUT))
    {
        m_config = *config;
        SetEvent(m_connectEvent);
    }
}

// See Bootloader.h for documentation of this method.
void Bootloader::UpdateImage(blfwkdll::Updater::PeripheralConfig *config, FlashFirmwareOption *option)
{
#ifdef DISABLE_QUICK_UPDATE
    if (!m_isConneted)
    {
        return;
    }
#endif
    DWORD state = WaitForEvents(0);
#ifdef _DEBUG
    PrintLog(_T("Events state = %d at UpdateImage()"), state);
#endif
    // if no event is triggered, or only ping event is triggered, set this event.
    if ((state == WAIT_OBJECT_0) || (state == WAIT_TIMEOUT))
    {
        if (config != NULL)
        {
            m_config = *config;
        }
        m_firmwareConfig = *option;
        m_abort = false;
        SetEvent(m_updateEvent);
    }
}

// See Bootloader.h for documentation of this method.
void Bootloader::Erase(EraseOption *option)
{
    if (!m_isConneted)
    {
        return;
    }

    DWORD state = WaitForEvents(0);
#ifdef _DEBUG
    PrintLog(_T("Events state = %d at Erase()"), state);
#endif
    // if no event is triggered, or only ping event is triggered, set this event.
    if ((state == WAIT_OBJECT_0) || (state == WAIT_TIMEOUT))
    {
        m_eraseOption = *option;
        SetEvent(m_eraseEvent);
    }
}

// See Bootloader.h for documentation of this method.
void Bootloader::ProgramOnce(ProgramOnceOption *option)
{
    if (!m_isConneted)
    {
        return;
    }

    DWORD state = WaitForEvents(0);
#ifdef _DEBUG
    PrintLog(_T("Events state = %d at ProgramOnce()"), state);
#endif
    // if no event is triggered, or only ping event is triggered, set this event.
    if ((state == WAIT_OBJECT_0) || (state == WAIT_TIMEOUT))
    {
        m_programOnceOption = *option;
        SetEvent(m_programOnceEvent);
    }
}

// See Bootloader.h for documentation of this method.
DWORD Bootloader::WaitForEvents(DWORD timeout)
{
    DWORD result;
    HANDLE eventsToWait[8] = {
        m_pingEvent,   m_statusEvent, m_resetEvent,       m_connectEvent,
        m_updateEvent, m_eraseEvent,  m_programOnceEvent, m_exitEvent,
    };

    result = WaitForMultipleObjects(8, eventsToWait, FALSE, timeout);

    switch (result)
    {
        case WAIT_OBJECT_0:
        case WAIT_OBJECT_0 + 1:
        case WAIT_OBJECT_0 + 2:
        case WAIT_OBJECT_0 + 3:
        case WAIT_OBJECT_0 + 4:
        case WAIT_OBJECT_0 + 5:
        case WAIT_OBJECT_0 + 6:
            m_isRunning = true;
            break;
        case WAIT_OBJECT_0 + 7:
            m_isExiting = true;
            break;
        default:
            break;
    }
    return result;
}

// See Bootloader.h for documentation of this method.
void Bootloader::ResetEvents(DWORD events)
{
    switch (events)
    {
        case WAIT_OBJECT_0:
            ResetEvent(m_pingEvent);
            break;
        case WAIT_OBJECT_0 + 1:
            ResetEvent(m_statusEvent);
            break;
        case WAIT_OBJECT_0 + 2:
            ResetEvent(m_resetEvent);
            break;
        case WAIT_OBJECT_0 + 3:
            ResetEvent(m_connectEvent);
            break;
        case WAIT_OBJECT_0 + 4:
            ResetEvent(m_updateEvent);
            break;
        case WAIT_OBJECT_0 + 5:
            ResetEvent(m_eraseEvent);
            break;
        case WAIT_OBJECT_0 + 6:
            ResetEvent(m_programOnceEvent);
            break;
        case WAIT_OBJECT_0 + 7:
            // Auto Reset is enabled for this event.
            break;
        default:
            break;
    }
}

//! \brief Chile thread function.
UINT ThreadFunc(LPVOID pParam)
{
    uint32_t responseWords[6];
    uint32_t status;
    Bootloader *pBootloader = (Bootloader *)pParam;

    while (!pBootloader->m_isExiting)
    {
        DWORD result = pBootloader->WaitForEvents(INFINITE);
        switch (result)
        {
            case WAIT_OBJECT_0: // Ping Operation
                try
                {
                    if (pBootloader->m_updater == NULL)
                    {
                        pBootloader->m_updater = new blfwkdll::Updater(&pBootloader->m_config);
                    }
                    else
                    {
                        if (pBootloader->m_config.peripheralType ==
                            blfwkdll::Updater::HOST_PERIPHERAL_TYPE_TAG::kHostPeripheralType_USB_HID)
                        {
                            // No ping packet for USB. So use getProperty 1 to test to
                            // communication.
                            pBootloader->m_updater->getProperty(
                                blfwkdll::Updater::PROPERTY_TAG::kPropertyTag_BootloaderVersion, responseWords);
                        }
                        else
                        {
                            // Send ping packet for serial peripherals.
                            pBootloader->m_updater->ping(0, 0, pBootloader->m_config.comPortSpeed);
                        }
                    }
                    if (!pBootloader->m_isConneted)
                    {
                        // Status is changed from disconnected to connected, send out the
                        // message.
                        pBootloader->m_isConneted = true;
                        ::PostMessage(*g_pMainDlg, USER_MS_CONNECTION_CHANGE, NULL, NULL);
                    }
                }
#ifdef _DEBUG
                catch (const std::exception &e)
#else
                catch (const std::exception &)
#endif
                {
#ifdef _DEBUG
                    // Print ping failed info when active config is debug.
                    PrintLog(_T("Error: Ping device failed(%s)."), CString(e.what()));
#endif
                    // If connection is failed via USB, we need to delete update and then
                    // re-enumerate it at the second attempt.
                    if ((pBootloader->m_config.peripheralType ==
                         blfwkdll::Updater::HOST_PERIPHERAL_TYPE_TAG::kHostPeripheralType_USB_HID) &&
                        (pBootloader->m_updater != NULL))
                    {
                        delete pBootloader->m_updater;
                        pBootloader->m_updater = NULL;
                    }
                    // Status is changed from connected to disconnected, send out the
                    // message.
                    if (pBootloader->m_isConneted)
                    {
                        pBootloader->m_isConneted = false;
                        ::PostMessage(*g_pMainDlg, USER_MS_CONNECTION_CHANGE, NULL, NULL);
                    }
                }
                break;

            case WAIT_OBJECT_0 + 1: // Update status
                if (pBootloader->m_updater == NULL)
                {
#ifdef _DEBUG
                    PrintLog(_T("Error: Invalid Bootloader.m_updater(NULL).\r\n"));
#else
                    PrintLog(_T("Error: Fail to communicate with device. Please reconnet the device.\r\n"));
#endif
                    break;
                }

                try
                {
                    pBootloader->m_updater->getProperty(blfwkdll::Updater::PROPERTY_TAG::kPropertyTag_BootloaderVersion,
                                                        responseWords);
                    pBootloader->m_targetInfo.targetVersion = responseWords[1];

                    pBootloader->m_targetInfo.procotolVersion = pBootloader->m_updater->getSerialProtocolVersion();

                    uint32_t flashSize = pBootloader->m_updater->getInternalFlashSize();
                    // flashSize == 0 means current target has no internal flash.
                    if (!flashSize)
                    {
                        pBootloader->m_hasInternalFlash = false;
                        pBootloader->m_targetInfo.flashSize = 0;
                        pBootloader->m_targetInfo.securityState = 0;
                    }
                    //  The target has internal flash.
                    else
                    {
                        pBootloader->m_hasInternalFlash = true;
                        pBootloader->m_targetInfo.flashSize = flashSize;

                        pBootloader->m_updater->getProperty(
                            blfwkdll::Updater::PROPERTY_TAG::kPropertyTag_FlashSecurityState, responseWords);
                        pBootloader->m_targetInfo.securityState = responseWords[1];
                    }
                    pBootloader->m_targetInfo.ramSize = pBootloader->m_updater->getInternalRAMSize();

                    pBootloader->m_updater->getProperty(blfwkdll::Updater::PROPERTY_TAG::kPropertyTag_ReservedRegions,
                                                        responseWords);
                    pBootloader->m_targetInfo.rsvRgnFlashStart = responseWords[1];
                    pBootloader->m_targetInfo.rsvRgnFlashEnd = responseWords[2];
                    pBootloader->m_targetInfo.rsvRgnRamStart = responseWords[3];
                    pBootloader->m_targetInfo.rsvRgnRamEnd = responseWords[4];

                    ::PostMessage(*g_pMainDlg, USER_MS_UPDATA_STATUS, NULL, NULL);
                }
                catch (const std::exception &e)
                {
                    PrintLog(_T("Error: Update status failed(%s).\r\n"), CString(e.what()));
                }
                break;

            case WAIT_OBJECT_0 + 2: // Reset
                if (pBootloader->m_updater == NULL)
                {
#ifdef _DEBUG
                    PrintLog(_T("Error: Invalid Bootloader.m_updater(NULL).\r\n"));
#else
                    PrintLog(_T("Error: Fail to communicate with device. Please reconnet the device.\r\n"));
#endif
                    break;
                }
                try
                {
                    pBootloader->m_updater->reset();
                    PrintLog(_T("Reset device successfully!"));
                }
                catch (const std::exception &e)
                {
                    PrintLog(_T("Error: Reset device failed(%s).\r\n"), e.what());
                }
                delete pBootloader->m_updater;
                pBootloader->m_updater = NULL;

                // Status is changed from connected to disconnected, send out the message.
                if (pBootloader->m_isConneted)
                {
                    pBootloader->m_isConneted = false;
                    ::PostMessage(*g_pMainDlg, USER_MS_CONNECTION_CHANGE, NULL, NULL);
                }
                break;

            case WAIT_OBJECT_0 + 3: // Connect
                try
                {
                    // Peripheral configs might be changed, So must re-declare a new one.
                    if (pBootloader->m_updater != NULL)
                        delete pBootloader->m_updater;

                    pBootloader->m_updater = new blfwkdll::Updater(&pBootloader->m_config);

                    // Status is changed from disconnected to connected, send out the
                    // message.
                    if (!pBootloader->m_isConneted)
                    {
                        pBootloader->m_isConneted = true;
                        ::PostMessage(*g_pMainDlg, USER_MS_CONNECTION_CHANGE, NULL, NULL);
                    }
                }
                catch (const std::exception &e)
                {
                    PrintLog(_T("Error: Connect device failed(%s).\r\n"), CString(e.what()));

                    if (pBootloader->m_isConneted)
                    {
                        pBootloader->m_isConneted = false;
                        ::PostMessage(*g_pMainDlg, USER_MS_CONNECTION_CHANGE, NULL, NULL);
                    }
                }
                break;

            case WAIT_OBJECT_0 + 4: // Update Image
                PrintLog(_T("Start update progress......"));
                pBootloader->progressCallback(0, 1, 1); // Reset the progress bar
#ifndef DISABLE_QUICK_UPDATE
                // If not connect to a device, try to connect firstly.
                if ((!pBootloader->m_isConneted) || (pBootloader->m_updater == NULL))
                {
                    bool cntStatus = false;
                    PrintLog(_T("No device is connected!\r\nTry to connect device......"));
                    // Try 5 attempts to connect device.
                    for (size_t retry = 0; retry < 5; retry++)
                    {
                        try
                        {
                            // If it is not NULL, delete it,  for the reason that peripheral
                            // configuration might be changed.
                            if (pBootloader->m_updater != NULL)
                                delete pBootloader->m_updater;

                            pBootloader->m_updater = new blfwkdll::Updater(&pBootloader->m_config);

                            PrintLog(_T("Connected to device successfully!"));

                            uint32_t flashSize = pBootloader->m_updater->getInternalFlashSize();
                            // flashSize == 0 means current target has no internal flash.
                            if (!flashSize)
                            {
                                pBootloader->m_hasInternalFlash = false;
                            }
                            else
                            {
                                pBootloader->m_hasInternalFlash = true;
                            }
                            // Connect opertion is sucessful.
                            cntStatus = true;
                            break;
                        }
                        catch (const std::exception &e)
                        {
                            PrintLog(_T("Error: Connect device failed(%s) at %d attempts."), CString(e.what()),
                                     retry + 1);
                            Sleep(500); // Wait 500ms.
                        }
                    }
                    if (cntStatus)
                    {
                        try
                        {
                            if (!pBootloader->m_hasInternalFlash)
                            {
                                pBootloader->m_targetInfo.securityState = 0;
                            }
                            else
                            {
                                pBootloader->m_updater->getProperty(
                                    blfwkdll::Updater::PROPERTY_TAG::kPropertyTag_FlashSecurityState, responseWords);
                                pBootloader->m_targetInfo.securityState = responseWords[1];
                            }

                            PrintLog(_T("Get device security state successfully(%s)."),
                                     pBootloader->m_targetInfo.securityState ? _T("SECURE") : _T("UNSECURE"));
                        }
                        catch (const std::exception &e)
                        {
                            PrintLog(_T("Error: Get device security state failed(%s)."), CString(e.what()));
                            goto UpdateError;
                        }
                    }
                    else
                    {
                        PrintLog(_T("Error: Try to connect device failed."));
                        goto UpdateError;
                    }
                }
#else
                if (pBootloader->m_updater == NULL)
                {
#ifdef _DEBUG
                    PrintLog(_T("Error: Invalid Bootloader.m_updater(NULL).\r\n"));
#else
                    PrintLog(_T("Error: Fail to communicate with device. Please reconnet the device.\r\n"));
#endif
                    break;
                }
#endif
                pBootloader->m_updater->registerCallback(pBootloader->progressCallback, &(pBootloader->m_abort));

                if (pBootloader->m_targetInfo.securityState)
                {
                    try
                    {
                        if (pBootloader->m_firmwareConfig.doErasetoUnlock)
                        {
                            PrintLog(_T("Unlock device by ERASEALL......"));
                            pBootloader->m_updater->eraseAllUnsecure();
                            PrintLog(_T("Device is unlocked!"));
                        }
                        else if (pBootloader->m_firmwareConfig.useKeytoUnlock)
                        {
                            PrintLog(_T("Unlock device by BACKDOORKEY......"));

                            pBootloader->m_updater->unlockWithKey(pBootloader->m_firmwareConfig.backdoorkey);
                            PrintLog(_T("Device is unlocked!"));
                        }
                        else
                        {
                            PrintLog(_T("Update directly without unlock device."));
                        }
                    }
                    catch (const std::exception &e)
                    {
                        PrintLog(_T("Error: Unlock device failed(%s)."), CString(e.what()));
                        goto UpdateError;
                    }
                }

                try
                {
                    PrintLog(_T("Updating image......"));
                    status = pBootloader->m_updater->flashFirmware(&pBootloader->m_firmwareConfig.imageFilePath,
                                                                   pBootloader->m_firmwareConfig.baseAddress,
                                                                   pBootloader->m_firmwareConfig.memoryId.first);
                    PrintLog(_T("Update image successfully!"));
                    if (status == Bootloader::KibbleStatusCode::KBL_Status_Success)
                    {
                        pBootloader->m_updater->reset();
                        PrintLog(_T("Reset device successfully!"));
                    }
                    else if (status = Bootloader::KibbleStatusCode::KBL_Status_AbortDataPhase)
                    {
                        pBootloader->progressCallback(100, 1, 1);
                    }
                    // for the left cases, an exception will be throw out.
                }
                catch (const std::exception &e)
                {
                    PrintLog(_T("Error: Update image failed(%s)."), CString(e.what()));
                    goto UpdateError;
                }
                PrintLog(_T("Update process is completed.\r\n"));

                delete pBootloader->m_updater;
                pBootloader->m_updater = NULL;

                // Status is changed from connected to disconnected, send out the message.
                if (pBootloader->m_isConneted)
                {
                    pBootloader->m_isConneted = false;
                    ::PostMessage(*g_pMainDlg, USER_MS_CONNECTION_CHANGE, NULL, NULL);
                }
                break;

            UpdateError:
                PrintLog(_T("Update process is stopped by error.\r\n"));
                break;

            case WAIT_OBJECT_0 + 5: // Erase operation
                if (pBootloader->m_updater == NULL)
                {
#ifdef _DEBUG
                    PrintLog(_T("Error: Invalid Bootloader.m_updater(NULL).\r\n"));
#else
                    PrintLog(_T("Error: Fail to communicate with device. Please reconnet the device.\r\n"));
#endif
                    break;
                }
                try
                {
                    switch (pBootloader->m_eraseOption.option)
                    {
                        case Bootloader::EraseOption::ERASE_OPERATION::erase_region:
                            pBootloader->m_updater->eraseRegion(pBootloader->m_eraseOption.startAddress,
                                                                pBootloader->m_eraseOption.length,
                                                                pBootloader->m_eraseOption.memoryId.first);
                            PrintLog(_T("Erase region from 0x%X to 0x%X of %s. Completed!"),
                                     pBootloader->m_eraseOption.startAddress,
                                     pBootloader->m_eraseOption.startAddress + pBootloader->m_eraseOption.length - 1,
                                     pBootloader->m_eraseOption.memoryId.second);
                            break;
                        case Bootloader::EraseOption::ERASE_OPERATION::erase_all:
                            pBootloader->m_updater->eraseAll(pBootloader->m_eraseOption.memoryId.first);
                            PrintLog(_T("Erase all memory of %s. Completed!"),
                                     pBootloader->m_eraseOption.memoryId.second);
                            break;
                        case Bootloader::EraseOption::ERASE_OPERATION::erase_all_and_unsecure:
                            pBootloader->m_updater->eraseAllUnsecure();
                            PrintLog(
                                _T("Erase all memory of internal flash and unsecore the ")
                                _T("device. Completed!"));
                            // If target has no internal flash, then no need to update the security status.
                            if (pBootloader->m_hasInternalFlash)
                            {
                                pBootloader->m_updater->getProperty(
                                    blfwkdll::Updater::PROPERTY_TAG::kPropertyTag_FlashSecurityState, responseWords);
                                pBootloader->m_targetInfo.securityState = responseWords[1];
                                ::PostMessage(*g_pMainDlg, USER_MS_UPDATA_STATUS, NULL, NULL);
                            }
                            break;
                        default:
                            break;
                    }
                }
                catch (const std::exception &e)
                {
                    PrintLog(_T("Error: Erase operation failed(%s).\r\n"), CString(e.what()));
                }
                break;

            case WAIT_OBJECT_0 + 6: // Program Once
                if (pBootloader->m_updater == NULL)
                {
#ifdef _DEBUG
                    PrintLog(_T("Error: Invalid Bootloader.m_updater(NULL).\r\n"));
#else
                    PrintLog(_T("Error: Fail to communicate with device. Please reconnet the device.\r\n"));
#endif
                    break;
                }
                try
                {
                    pBootloader->m_updater->programOnce(
                        pBootloader->m_programOnceOption.index, pBootloader->m_programOnceOption.byteCount,
                        pBootloader->m_programOnceOption.data, pBootloader->m_programOnceOption.isLsb);
                    PrintLog(_T("Program IFR. Completed!"));
                }
                catch (const std::exception &e)
                {
                    PrintLog(_T("Error: Program IFR failed(%s).\r\n"), CString(e.what()));
                }
                break;

            default:
                break;
        }
        pBootloader->m_isRunning = false;
        pBootloader->ResetEvents(result);
    }
    return 0;
}
