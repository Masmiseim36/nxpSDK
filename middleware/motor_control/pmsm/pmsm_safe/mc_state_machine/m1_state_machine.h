/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _M1_STATE_MACHINE_H_
#define _M1_STATE_MACHINE_H_

#include "m1_state_routines.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief   State-machine initialization routine, which must be called during
 *          the MCU initialization phase.
 *
 * @param   void
 *
 * @return  none
 */
void M1_fsStateMachine_AR(void);

/*!
 * @brief   State-machine portion, which must be executed in high-priority
 *          interrupt routine.
 *
 * @param   void
 *
 * @return  none
 */
void M1_fsStateMachine_FL(void);

/*!
 * @brief   State-machine portion, which should be executed in low-priority
 *          interrupt routine.
 *
 * @param   void
 *
 * @return  none
 */
void M1_fsStateMachine_SL(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _M1_STATE_MACHINE_H_ */

