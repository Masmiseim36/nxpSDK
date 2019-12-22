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
 * ex_sss_ports.h:  Default ports being used in Examples and test cases
 *
 * $Date: Mar 10, 2019 $
 * $Author: ing05193 $
 * $Revision$
 */

#ifndef SSS_EX_INC_EX_SSS_PORTS_H_
#define SSS_EX_INC_EX_SSS_PORTS_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

#define EX_SSS_BOOT_SSS_PORT "EX_SSS_BOOT_SSS_PORT"
#define EX_SSS_BOOT_SSS_COMPORT_DEFAULT "\\\\.\\COM19"
#define EX_SSS_BOOT_SSS_SOCKET_HOSTNAME_DEFAULT "127.0.0.1"
#define EX_SSS_BOOT_SSS_SOCKET_PORTNUMBER_DEFAULT 8050
#define EX_SSS_BOOT_SSS_SOCKET_PORTSZ_DEFAULT "8050"
#define EX_SSS_BOOT_SSS_SOCKETPORT_DEFAULT  \
    EX_SSS_BOOT_SSS_SOCKET_HOSTNAME_DEFAULT \
    ":" EX_SSS_BOOT_SSS_SOCKET_PORTSZ_DEFAULT

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

#endif /* SSS_EX_INC_EX_SSS_PORTS_H_ */
