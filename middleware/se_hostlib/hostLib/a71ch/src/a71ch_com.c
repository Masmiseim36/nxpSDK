/**
* @file a71ch_com.c
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
* Implementation of basic communication functionality between Host and A71CH.
* @par History
* 1.0   1-oct-2016 : Initial version
*
*****************************************************************************/
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <a71ch_const.h>
#include "ax_api.h"
#include "ax_util.h"
#include "sm_apdu.h"
#include "sm_errors.h"

#ifdef TDA8029_UART
#include "smUart.h"
#include "smComAlpar.h"
#elif defined(I2C)
#include "smComSCI2C.h"
#elif defined(SPI)
#include "smComSCSPI.h"
#elif defined(PCSC)
#include "smComPCSC.h"
#elif defined(IPC)
#include "smComIpc.h"
#elif defined(RJCT_SOCKET)
#include "smComSocket.h"
#elif defined(RJCT_VCOM)
#include "smComSerial.h"
#else
#error "Select Communication interface"
#endif

#include "global_platf.h"

/// @cond Optional diagnostics functionality
// #define FLOW_VERBOSE
#ifdef FLOW_VERBOSE
#define FPRINTF(...) printf (__VA_ARGS__)
#else
#define FPRINTF(...)
#endif
/// @endcond


#if defined(RJCT_SOCKET) || defined (RJCT_JRCP)
static U16 getSocketParams(const char *arg, U8 *szServer, U16 szServerLen, unsigned int *port)
{
    // the IP address is in format a.b.c.d:port, e.g. 10.0.0.1:8080
    int nSuccess;

    if (strlen(arg) >= szServerLen) {
        return ERR_BUF_TOO_SMALL;
    }

    // First attempt at parsing: server IP-address passed, sscanf will return 2 upon successfull parsing
    nSuccess = sscanf(arg, "%15[0-9.]:%5u[0-9]", szServer, (unsigned int *)port);
    if (nSuccess == 2) {
        return SW_OK;
    }
    else {
        // Second attempt at parsing: server name passed instead of IP-address
        unsigned int i;
        int fColonFound = 0;

        for (i = 0; i < strlen(arg); i++) {
            if (arg[i] == ':') {
                szServer[i] = 0;
                fColonFound = 1;
                // PRINTF("servername: %s\r\n", szServer);
                break;
            }
            else {
                szServer[i] = arg[i];
            }
        }

        if ((fColonFound == 1) && (i != 0)) {
            nSuccess = sscanf(&arg[i], ":%5u[0-9]", (unsigned int *)port);
            if (nSuccess == 1) {
                return SW_OK;
            }
        }
    }
    return ERR_NO_VALID_IP_PORT_PATTERN;
}

/**
* Establishes communication with the Security Module via a Remote JC Terminal Server
* (RJCT-Server).
* Next it will invoke ::SM_Connect and select the A71CH applet on the Secure Module
*
* \note Because connecting via an RJCT-server requires an extra parameter (the server IP:Port)
* an additional function is required on top of ::SM_Connect
*
* @param[in,out] connectString ip:port as string
* @param[in,out] commState
* @param[in,out] atr
* @param[in,out] atrLen
*
* @retval ::SW_OK Upon successful execution
*/
U16 SM_RjctConnect(const char *connectString, SmCommState_t *commState, U8 *atr, U16 *atrLen)
{
    U8 szServer[128];
    U16 szServerLen = sizeof(szServer);
    U16 rv = 0;
    unsigned int port = 0;

    FPRINTF("Connection to secure element over socket to %s\r\n", connectString);

    rv = getSocketParams(connectString, szServer, szServerLen, (unsigned int*) &port);
    if (rv != SW_OK)
    {
        return rv;
    }

    rv = smComSocket_Open(szServer, (U16)port, atr, atrLen);
    if (rv != SMCOM_OK)
    {
        FPRINTF("Error on smComSocket_Open: 0x%04X\r\n", rv);
        return ERR_CONNECT_LINK_FAILED;
    }

    rv = SM_Connect(commState, atr, atrLen);
    return rv;
}
#endif // RJCT_SOCKET

#ifdef RJCT_VCOM
U16 SM_RjctConnect(const char *connectString, SmCommState_t *commState, U8 *atr, U16 *atrLen)
{
    U32 status;

    status = smComVCom_Open(connectString);

    if (status == 0)
    {
        status = smComVCom_GetATR(atr,atrLen);
        if (status == 0)
        {
            status = (U16)SM_Connect(commState, atr, atrLen);
        }
    }
    else
    {
        *atrLen = 0;
    }

    return (U16)status;
}
#endif // RJCT_VCOM

/**
* Establishes the communication with the Security Module (SM) at the link level and
* selects the A71CH applet on the SM. The physical communication layer used (e.g. I2C)
* is determined at compilation time.
*
* @param[in,out] commState
* @param[in,out] atr
* @param[in,out] atrLen
*
* @retval ::SW_OK Upon successful execution
*/
U16 SM_Connect(SmCommState_t *commState, U8 *atr, U16 *atrLen)
{
    U16 sw = SW_OK;
#if !defined(IPC)
    char appletName[] = "a71ch"; // 0x61.37.31.63.68
    U16 selectResponseDataLen = 0;
    U8 selectResponseData[256];
    U16 uartBR = 0;
    U16 t1BR = 0;
#endif
#ifdef TDA8029_UART
    U32 status = 0;
#endif

    assert(commState!=NULL);
#ifdef TDA8029_UART
    assert(atr!=NULL);
    assert(atrLen!=NULL);
    assert((*atrLen)>33);

    smComAlpar_Init();
    status = smComAlpar_AtrT1Configure(ALPAR_T1_BAUDRATE_MAX, atr, atrLen, &uartBR, &t1BR);
    if (status != SMCOM_ALPAR_OK )
    {
        commState->param1 = 0;
        commState->param2 = 0;
        FPRINTF("smComAlpar_AtrT1Configure failed: 0x%08X\r\n", status);
        return ERR_CONNECT_LINK_FAILED;
    }
#elif defined(I2C)
    sw = smComSCI2C_Open(ESTABLISH_SCI2C, 0x00, atr, atrLen);
#elif defined(SPI)
    smComSCSPI_Init(ESTABLISH_SCI2C, 0x00, atr, atrLen);
#elif defined(PCSC)
    sw = smComPCSC_Open(0, atr, atrLen);
#elif defined(IPC)
    sw = smComIpc_Open(atr, atrLen, &(commState->hostLibVersion), &(commState->appletVersion), &(commState->sbVersion));
#elif defined(RJCT_SOCKET)
    AX_UNUSED_ARG(atr);
    AX_UNUSED_ARG(atrLen);
#endif // TDA8029_UART

#if !defined(IPC)
    commState->param1 = t1BR;
    commState->param2 = uartBR;
    commState->hostLibVersion = (AX_HOST_LIB_MAJOR << 8) + AX_HOST_LIB_MINOR;
    commState->appletVersion = 0xFFFF;
    commState->sbVersion = 0xFFFF;

    if (sw == SMCOM_OK)
    {
        selectResponseDataLen = sizeof(selectResponseData);
        sw = GP_Select((U8*) &appletName, (U8) strlen(appletName),
                selectResponseData, &selectResponseDataLen);

        if (sw == SW_FILE_NOT_FOUND)
        {
            // Applet can not be selected (most likely it is simply not installed)
            return sw;
        }
        else if (sw != SW_OK)
        {
            sw = ERR_CONNECT_SELECT_FAILED;
        }
        else
        {
#ifdef FLOW_VERBOSE
            PRINTF("selectResponseDataLen: %d\r\n", selectResponseDataLen);
            {
                int i = 0;
                for (i=0; i<selectResponseDataLen; i++)
                {
                    PRINTF("0x%02X:", selectResponseData[i]);
                }
                PRINTF("\r\n");
            }
#endif
            if (selectResponseDataLen >= 2)
            {
                if (selectResponseDataLen == 4)
                {
                    commState->appletVersion = (selectResponseData[0] << 8) + selectResponseData[1];
                    commState->sbVersion = (selectResponseData[2] << 8) + selectResponseData[3];
                }
                else if (selectResponseDataLen == 2)
                {
                    commState->appletVersion = (selectResponseData[0] << 8) + selectResponseData[1];
                    commState->sbVersion = 0x0000;
                }
            }
            else
            {
                sw = ERR_CONNECT_SELECT_FAILED;
            }
        }
    }
#endif // !defined(IPC)
    return sw;
}

/**
 * Resumes the communication with the Security Module including the secure messaging from the previously
 * retrieved communication state and SCP03 session state.
 *
 * @param[in] commState   communication state
 * @param[in] scp03State  SCP03 session state
 *
 * @retval ::SW_OK Upon successful execution
 */
U16 SM_ResumeConnection(SmCommState_t *commState, Scp03SessionState_t *scp03State)
{
    U16 sw = SW_OK;
#if defined(I2C)
    U8 atrDummy[32];
    U16 atrDummyLen = sizeof(atrDummy);
#endif

    FPRINTF("** Session State re-used **\r\n");
    FPRINTF("***************************\r\n");
#ifdef TDA8029_UART
    smComAlpar_Init();
    smUartSetBaudRate(commState->param2);
#elif defined(I2C)
    sw = smComSCI2C_Open(RESUME_SCI2C, (U8)(commState->param1), atrDummy, &atrDummyLen);
#else
    AX_UNUSED_ARG(commState);
#endif

#if !defined(NO_SECURE_CHANNEL_SUPPORT)
    SCP_SetScpSessionState(scp03State);
    DEV_SetResumeHostChannelState();
#endif
    return sw;
}

/**
 * Closes the communication with the Security Module
 * A new connection can be established by calling ::SM_Connect
 *
 * @param[in] mode Specific information that may be required on the link layer
 *
 * @retval ::SW_OK Upon successful execution
 */
U16 SM_Close(U8 mode)
{
    U16 sw = SW_OK;

#if defined(I2C)
    sw = smComSCI2C_Close(mode);
#elif defined(SPI)
    sw = smComSCSPI_Close(mode);
#elif defined(PCSC)
    sw = smComPCSC_Close(mode);
#elif defined(IPC)
    AX_UNUSED_ARG(mode);
    sw = smComIpc_Close();
#elif defined(RJCT_SOCKET)
    AX_UNUSED_ARG(mode);
    sw = smComSocket_Close();
#endif

    return sw;
}

/**
 * Sends the command APDU to the Secure Module and retrieves the response APDU.
 * The latter consists of the concatenation of the response data (possibly none) and the status word (2 bytes).
 *
 * The command APDU and response APDU are not interpreted by the host library.
 *
 * The command/response APDU sizes must lay within the APDU size limitations
 *
 * @param[in] cmd   command APDU
 * @param[in] cmdLen length (in byte) of \p cmd
 * @param[in,out] resp  response APDU (response data || response status word)
 * @param[in,out] respLen IN: Length of resp buffer (\p resp) provided; OUT: effective length of response retrieved.
 *
 * @retval ::SW_OK Upon successful execution
 */
U16 SM_SendAPDU(U8 *cmd, U16 cmdLen, U8 *resp, U16 *respLen)
{
    U32 status = 0;
    U32 respLenLocal = *respLen;

    status = smCom_TransceiveRaw(cmd, cmdLen, resp, &respLenLocal);
    *respLen = (U16)respLenLocal;

    return (U16) status;
}

#if defined(IPC)
U16 SM_LockChannel()
{
    return smComIpc_LockChannel();
}

U16 SM_UnlockChannel()
{
    return smComIpc_UnlockChannel();
}
#endif
