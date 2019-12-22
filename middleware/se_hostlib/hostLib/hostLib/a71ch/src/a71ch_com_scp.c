/**
* @file a71ch_com_scp.c
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2016,2018 NXP
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

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include <sm_const.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sm_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "sm_printf.h"
#include "sm_types.h"
#include "ax_common.h"
#include "ax_scp.h"
#include "a71ch_util.h"

#if SSS_HAVE_SSCP
#include "fsl_sscp_a71ch.h"
#endif

//Also do select after opening the connection
#define OPEN_AND_SELECT 0


#ifdef TDA8029_UART
#include "smComAlpar.h"
#include "smUart.h"
#endif
#if defined(SCI2C)
#include "smComSCI2C.h"
#endif
#if defined(SPI)
#include "smComSCSPI.h"
#endif
#if defined(PCSC)
#include "smComPCSC.h"
#endif
#if defined(IPC)
#include "smComIpc.h"
#endif
#if defined(SMCOM_JRCP_V1)
#include "smComSocket.h"
#endif
#if defined(SMCOM_JRCP_V2)
#include "smComJRCP.h"
#endif
#if defined(RJCT_VCOM)
#include "smComSerial.h"
#endif
#if defined(T1oI2C)
#include "smComT1oI2C.h"
#endif

#include "global_platf.h"

/// @cond Optional diagnostics functionality
// #define FLOW_VERBOSE
#ifdef FLOW_VERBOSE
#define FPRINTF(...) printf(__VA_ARGS__)
#else
#define FPRINTF(...)
#endif
/// @endcond

/**
 * Resumes the communication with the Security Module including the secure messaging from the previously
 * retrieved communication state and SCP03 session state.
 *
 * @param[in] commState   communication state
 * @param[in] scp03State  SCP03 session state
 *
 * @retval ::SW_OK Upon successful execution
 */
U16 SM_ResumeConnection(
    SmCommState_t *commState, Scp03SessionState_t *scp03State)
{
    U16 sw = SW_OK;
#if defined(SCI2C)
    U8 atrDummy[32];
    U16 atrDummyLen = sizeof(atrDummy);
#endif

    FPRINTF("** Session State re-used **\r\n");
    FPRINTF("***************************\r\n");
#ifdef TDA8029_UART
    smComAlpar_Init();
    smUartSetBaudRate(commState->param2);
#elif defined(SCI2C)
    sw = smComSCI2C_Open(
        RESUME_SCI2C, (U8)(commState->param1), atrDummy, &atrDummyLen);
#else
    AX_UNUSED_ARG(commState);
#endif

#if !defined(NO_SECURE_CHANNEL_SUPPORT)
    SCP_SetScpSessionState(scp03State);
    DEV_SetResumeHostChannelState();
#endif
    return sw;
}

