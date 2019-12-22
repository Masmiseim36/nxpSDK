/**
 * @file global_platf.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 *
 * @par History
 *
 */

#ifndef _GLOBAL_PLATF_
#define _GLOBAL_PLATF_

#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLA_ISO7816                   (0x00)  //!< ISO7816-4 defined CLA byte

#define INS_GP_INITIALIZE_UPDATE      (0x50)  //!< Global platform defined instruction
#define INS_GP_EXTERNAL_AUTHENTICATE  (0x82)  //!< Global platform defined instruction
#define INS_GP_SELECT                 (0xA4)  //!< Global platform defined instruction
#define INS_GP_PUT_KEY                (0xD8)  //!< Global platform defined instruction

U16 GP_Select(U8 *appletName, U16 appletNameLen, U8 *response, U16 *responseLen);
U16 GP_GetCLAppletVersion(U8 *appletVersion, U16 *verionLength);
#ifdef __cplusplus
}
#endif
#endif
