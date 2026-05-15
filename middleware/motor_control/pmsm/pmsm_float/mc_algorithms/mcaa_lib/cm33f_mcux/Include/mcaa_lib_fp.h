/*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024-2026 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
*
*/
/*!
*
* @brief Main MCAA_LIB header file for devices with FPU.
*
*******************************************************************************/
#ifndef _MCAA_LIB_FLT_H_
#define _MCAA_LIB_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "MCAA_PpAssist_FLT.h"
#include "MCAA_EstimRL_FLT.h"
#include "MCAA_EstimBJ_FLT.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MCAA_PpAssistInit_FLT(psParam, psCtrl)               \
        MCAA_PpAssistInit_FLT_C(psParam, psCtrl)
#define MCAA_PpAssist_FLT(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq) \
        MCAA_PpAssist_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq)
#define MCAA_EstimRLInit_FLT(psParam, psCtrl, psAdvTune)      \
        MCAA_EstimRLInit_FLT_C(psParam, psCtrl, psAdvTune)
#define MCAA_EstimRL_FLT(fltUDcBus, psIAlBeFbck, psCtrl, psParam, psAdvTune, psUAlBeReq)  \
        MCAA_EstimRL_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, psParam, psAdvTune, psUAlBeReq)
#define MCAA_EstimBJInit_FLT(psParam, psCtrl)               \
        MCAA_EstimBJInit_FLT_C(psParam, psCtrl)
#define MCAA_EstimBJ_FLT(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq) \
        MCAA_EstimBJ_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq)

#if defined(__cplusplus)
}
#endif

#endif /* _MCAA_LIB_FLT_H_ */
