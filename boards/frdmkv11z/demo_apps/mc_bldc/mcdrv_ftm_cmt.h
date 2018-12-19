/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_FTM_CMT_H_
#define _MCDRV_FTM_CMT_H_

#include "mlib.h"
#include "gflib.h"
#include "gdflib.h"
#include "gmclib.h"

#include "gmclib_types.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* init sensors/actuators pointers */
#define M1_SET_PTR_CNT_ACT(par1) (g_sM1CmtTmr.pui16FtmCntAct = &(par1))
#define M1_SET_PTR_VALUE_ACT(par1) (g_sM1CmtTmr.pui16FtmValueAct = &(par1))

typedef struct _mcdrv_ftm_cmt
{
    FTM_Type *pui32FtmBase;     /* pointer FlexTimer base address */
    uint16_t *pui16FtmCntAct;   /* pointer to actual value of FTM counter */
    uint16_t *pui16FtmValueAct; /* pointer to actual value of FTM value register */
    uint16_t ui16ChannelNum;    /* number of FTM channel used for compare event */
} mcdrv_ftm_cmt_t;

typedef struct _mcdrv_ftm_cmt_init
{
    FTM_Type *pui32FtmBase;  /* pointer FlexTimer base address */
    uint16_t ui16ChannelNum; /* number of FTM channel used for compare event */
} mcdrv_ftm_cmt_init_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initialization function of asynchronous time event
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmCmtInit(mcdrv_ftm_cmt_t *this, mcdrv_ftm_cmt_init_t *init);

/*!
 * @brief Function read actual values of FTM counter and value register
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmCmtGet(mcdrv_ftm_cmt_t *this);

/*!
 * @brief Function update FTM value register
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmCmtSet(mcdrv_ftm_cmt_t *this, uint16_t ui16TimeNew);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_FTM_CMT_H_ */

