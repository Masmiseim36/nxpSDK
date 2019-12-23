/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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
 * @brief Initialization function of asynchronous time event
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmCmtInit(mcdrv_ftm_cmt_t *this, mcdrv_ftm_cmt_init_t *init)
{
    s_statusPass = TRUE;

    this->pui32FtmBase = init->pui32FtmBase;     /* FTM Base address */
    this->ui16ChannelNum = init->ui16ChannelNum; /* FTM channel number */

    return (s_statusPass);
}

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

