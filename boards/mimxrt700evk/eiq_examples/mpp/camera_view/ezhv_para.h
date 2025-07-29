/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EZHV_PARA_
#define _EZHV_PARA_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief definition of EZH-V APIs */
typedef enum _ezhv_api_idx
{
    kEZHV_API_flexioDvp,
} EZHV_ApiIdx_t;

/*! @brief parameters to run which api with given parameters */
typedef struct tag_EzhvSharedData
{
    uint32_t apiIdx;   /*!< value from EZHV_ApiIdx_t */
    uint32_t paraAddr; /*!< &paraAddr is the start location of paras */
} EZHV_Para_t;


/*! @brief definition for paras of display video port API */
typedef struct _ezhv_para_dvp_t
{
    uint32_t len; /*!< buffer length, bytes */
    void   *pBuf; /*!< buffer pointer */
    //uint32_t stride;
} ezhv_para_dvp_t;

#endif
