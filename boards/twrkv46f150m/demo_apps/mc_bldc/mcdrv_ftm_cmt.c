/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_ftm_cmt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function read actual values of FTM counter and value register
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmCmtGet(mcdrv_ftm_cmt_t *this)
{
    s_statusPass = TRUE;

    /* read actual values of counter and defined value register */
    *this->pui16FtmCntAct = this->pui32FtmBase->CNT;
    *this->pui16FtmValueAct = this->pui32FtmBase->CONTROLS[this->ui16ChannelNum].CnV;

    return (s_statusPass);
}

/*!
 * @brief Function update FTM value register
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmCmtSet(mcdrv_ftm_cmt_t *this, uint16_t ui16TimeNew)
{
    s_statusPass = TRUE;

    /* update defined value register */
    this->pui32FtmBase->CONTROLS[this->ui16ChannelNum].CnV = ui16TimeNew;

    return (s_statusPass);
}

