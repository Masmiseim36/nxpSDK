/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <fsl_sss_sscp.h>
#include <stdio.h>

#include "ex_sss_boot_int.h"
#include "nxLog_App.h"

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH

/* ************************************************************************** */
/* Local Defines                                                              */
/* ************************************************************************** */

/* ************************************************************************** */
/* Structures and Typedefs                                                    */
/* ************************************************************************** */

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Static function declarations                                               */
/* ************************************************************************** */

/* ************************************************************************** */
/* Public Functions                                                           */
/* ************************************************************************** */

sss_status_t ex_sss_boot_a71ch_open(
    ex_sss_boot_ctx_t *pCtx, const char *portName)
{
    sss_status_t status = kStatus_SSS_Fail;

    SE_Connect_Ctx_t sessionCtxt = {0};

#if RJCT_VCOM
    sessionCtxt.connType = kType_SE_Conn_Type_VCOM;
    sessionCtxt.portName = portName;
#endif
#if defined(SMCOM_JRCP_V1)
    sessionCtxt.connType = kType_SE_Conn_Type_JRCP_V1;
#endif
#if defined(SMCOM_JRCP_V2)
    sessionCtxt.connType = kType_SE_Conn_Type_JRCP_V2;
#endif
#if defined(SMCOM_JRCP_V1) || defined(SMCOM_JRCP_V2)
    sessionCtxt.portName = portName;
#endif

#if RJCT_VCOM
    sessionCtxt.connType = kType_SE_Conn_Type_VCOM;
    sessionCtxt.portName = portName;
#endif
#if defined(SMCOM_JRCP_V1) || defined(SMCOM_JRCP_V2)
    sessionCtxt.connType = kType_SE_Conn_Type_JRCP_V2;
    sessionCtxt.portName = portName;
#endif
#if defined(SMCOM_PCSC)
    sessionCtxt.connType = kType_SE_Conn_Type_PCSC;
    sessionCtxt.portName = portName;
#endif

    status = sss_session_open(&pCtx->session,
        kType_SSS_SE_A71CH,
        0,
        kSSS_ConnectionType_Plain,
        &sessionCtxt);
    if (kStatus_SSS_Success != status) {
        LOG_E("sss_session_open failed");
        goto cleanup;
    }

cleanup:
    return status;
}

/* ************************************************************************** */
/* Private Functions                                                          */
/* ************************************************************************** */

#endif
