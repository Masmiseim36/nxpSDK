/*
 * Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/** @file
 *
 * ex_sss_boot_connectstring.c:  *The purpose and scope of this file*
 *
 * Project:  SecureIoTMW-Debug@appboot-top-eclipse_x86
 *
 * $Date: Mar 10, 2019 $
 * $Author: ing05193 $
 * $Revision$
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <ex_sss_boot.h>
#include <nxLog_App.h>
#include <sm_types.h>
#include <stdlib.h>
#include <string.h>

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

const char gszCOMPortDefault[] = EX_SSS_BOOT_SSS_COMPORT_DEFAULT;
const char gszSocketPortDefault[] = EX_SSS_BOOT_SSS_SOCKETPORT_DEFAULT;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */

sss_status_t ex_sss_boot_connectstring(
    int argc, const char *argv[], const char **pPortName)
{
    const char *portName = NULL;
    sss_status_t status = kStatus_SSS_Success;
#if defined(_WIN32) && defined(WIN32) && defined(DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

#if !AX_EMBEDDED
    if (argv != NULL) {
        LOG_I("Running %s", argv[0]);
    }
    if (argc > 1 && argv != NULL) {
        portName = argv[argc - 1]; /* last entry, deemed as port name */
        LOG_I("Using PortName='%s' (CLI)", portName);
    }
    else
#endif
    {
        const char *portName_env = getenv(EX_SSS_BOOT_SSS_PORT);
        if (portName_env != NULL) {
            portName = portName_env;
            LOG_I(
                "Using PortName='%s' (ENV=%s)", portName, EX_SSS_BOOT_SSS_PORT);
        }
    }

    if (portName == NULL) {
#if RJCT_VCOM
        portName = gszCOMPortDefault;
        LOG_I("Using PortName='%s' (gszCOMPortDefault)", portName);
#elif SMCOM_JRCP_V1 || SMCOM_JRCP_V2
        portName = gszSocketPortDefault;
        LOG_I("Using PortName='%s' (gszSocketPortDefault)", portName);
#else
        status = kStatus_SSS_Success;
#endif

#if AX_EMBEDDED
        /* FINE. To be moved to boot direct */
#else
        LOG_I(
            "If you want to over-ride the selection, use ENV=%s or pass in "
            "command line arguments.",
            EX_SSS_BOOT_SSS_PORT);
#endif
    }

    if (status == kStatus_SSS_Success && pPortName != NULL) {
        *pPortName = portName;
    }
    return status;
}

bool ex_sss_boot_isSerialPortName(const char *portName)
{
    bool is_vcom = FALSE;
#if RJCT_VCOM
    if (portName == NULL) {
        is_vcom = FALSE;
    }
    else if (0 == strncmp("COM", portName, sizeof("COM") - 1)) {
        is_vcom = TRUE;
    }
    else if (0 == strncmp("\\\\.\\COM", portName, sizeof("\\\\.\\COM") - 1)) {
        is_vcom = TRUE;
    }
    else if (0 == strncmp("/tty/", portName, sizeof("/tty/") - 1)) {
        is_vcom = TRUE;
    }
    else if (0 == strncmp("/dev/tty.", portName, sizeof("/dev/tty.") - 1)) {
        is_vcom = TRUE;
    }
#endif
    return is_vcom;
}

bool ex_sss_boot_isSocketPortName(const char *portName)
{
    bool is_socket = FALSE;
#if SMCOM_JRCP_V1 || SMCOM_JRCP_V2
    if (portName == NULL) {
        is_socket = FALSE;
    }
    else if (NULL != strchr(portName, ':')) {
        is_socket = TRUE;
    }
#endif
    return is_socket;
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
