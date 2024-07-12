/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef _CONTROLLER_IND_RESET_H_
#define _CONTROLLER_IND_RESET_H_

#include "fsl_common.h"
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))
typedef enum
{
    IND_RESET_DISABLE,
    IND_RESET_OOB,
    IND_RESET_INBAND
} ind_reset_mode_t;

#endif /*ENABLE_BT_IND_RESET*/

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
#if (defined(CONFIG_BT_IND_RESET) && (CONFIG_BT_IND_RESET > 0U))

void controler_ind_reset(ind_reset_mode_t ind_reset);

#endif /*ENABLE_BT_IND_RESET*/

#if defined(__cplusplus)
}
#endif

/*! @} */
#endif /* _CONTROLLER_IND_RESET_H_ */
