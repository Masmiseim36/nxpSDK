/**
* @file sm_api.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2018 NXP
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
* This file is the communication specific Host API of the A70CM/CI and A71CH secure module.
* It also customizes the Generic Ax library for this specific product instance
* @par History
* 1.0   27-march-2014 : Initial version
*        7-may-2017   : Unified version for A70CM, CI and A71CH
*
*****************************************************************************/
#ifndef _SM_API_
#define _SM_API_

#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <PlugAndTrust_HostLib_Ver.h>

#define AX_HOST_LIB_MAJOR (PLUGANDTRUST_HOSTLIB_VER_MAJOR)  //!< Major number A71CH Host Library
#define AX_HOST_LIB_MINOR (PLUGANDTRUST_HOSTLIB_VER_MINOR)  //!< Minor (High Nibble)/Patch number (Low Nibble) of A71CH Host Library

/*!
* @addtogroup sss_sscp_a71ch
* @{
*/

typedef enum
{
    kType_SE_Conn_Type_NONE,
    kType_SE_Conn_Type___UNUSED,
    kType_SE_Conn_Type_SCII2C,
    kType_SE_Conn_Type_VCOM,
    kType_SE_Conn_Type_JRCP_V1,
    kType_SE_Conn_Type_JRCP_V2,
    kType_SE_Conn_Type_T1oI2C,
    kType_SE_Conn_Type_NFC,
    kType_SE_Conn_Type_Tunnel,
    kType_SE_Conn_Type_LAST
} SEConnType_t;

/**
 * Contains the information required to resume a connection with the Security Module.
 * Its content is only to be interpreted by the Host Library.
 * The semantics of the param1 and param2 fields depends on the link layer.
 */
typedef struct {
    U16 connType;
    U16 param1;          //!< Useage depends on link layer
    U16 param2;          //!< Useage depends on link layer
    U16 hostLibVersion;  //!< MSByte contains major version (::AX_HOST_LIB_MAJOR); LSByte contains minor version of HostLib (::AX_HOST_LIB_MINOR)
    U32 appletVersion;   /*!< MSByte contains major version;
                              3 leading bits of LSByte contains minor version of Applet;
                              Last bit of LSByte encodes whether Applet is in Debug Mode, a '1' means 'Debug Mode' is available */
    U16 sbVersion;       //!< Expected to be 0x0000
} SmCommState_t;

/** \name Communication functions
   @{ */
U16 SM_Close(U8 mode);
U16 SM_Connect(SmCommState_t *commState, U8 *atr, U16 *atrLen);
U16 SM_ConnectWithAID(SmCommState_t *commState, U8* appletAID, U16 appletAIDLen, U8 *atr, U16 *atrLen);
U16 SM_RjctConnect(const char *connectString, SmCommState_t *commState, U8 *atr, U16 *atrLen);
U16 SM_RjctConnectWithAID(const char *connectString, SmCommState_t *commState, U8* appletAID, U16 appletAIDLen, U8 *atr, U16 *atrLen);

U16 SM_SendAPDU(U8 *cmd, U16 cmdLen, U8 *resp, U16 *respLen);
/** @}*/

#ifdef __cplusplus
}
#endif
#endif //_SM_API_
