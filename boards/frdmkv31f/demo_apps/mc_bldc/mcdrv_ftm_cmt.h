/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

