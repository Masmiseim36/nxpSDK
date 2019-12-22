/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
 
/*! @brief Get timer difference */
#define GET_TIME_DIFF(start, end) end - start

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 
void InitTimer (void);

int GetTimeInUS(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _TIMER_H_ */
