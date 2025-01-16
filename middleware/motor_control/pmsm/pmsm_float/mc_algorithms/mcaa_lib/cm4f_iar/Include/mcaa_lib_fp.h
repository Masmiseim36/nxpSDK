/*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
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
#include "MCAA_EstimRL_FLT.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MCAA_EstimRLInit_FLT(u32SamplingFreq, psParam, psCtrl)                 \
        MCAA_EstimRLInit_FLT_C(u32SamplingFreq, psParam, psCtrl)
#define MCAA_EstimRL_FLT(fltUDcBus, psIAlBeFbck, psCtrl, psParam, psUAlBeReq)  \
        MCAA_EstimRL_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, psParam, psUAlBeReq)

#if defined(__cplusplus)
}
#endif

#endif /* _MCAA_LIB_FLT_H_ */
