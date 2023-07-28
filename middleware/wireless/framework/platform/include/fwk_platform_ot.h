/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_PLATFORM_OT_H_
#define _FWK_PLATFORM_OT_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*                                Public types                                */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Low level initialization for OpenThread/15.4, can be used to initialize 15.4 Controller
 *
 * \return int return status: >=0 for success, <0 for errors
 */
int PLATFORM_InitOt(void);

#ifdef __cplusplus
}
#endif

#endif /* _FWK_PLATFORM_OT_H_ */