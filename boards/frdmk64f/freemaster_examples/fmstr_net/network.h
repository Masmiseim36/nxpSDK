/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Declarations
 */

#ifndef __FMSTR_NETWORK_H
#define __FMSTR_NETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Functions definitions
 ******************************************************************************/

/* Network example application functions */
void Network_Init(uint32_t csrClock_Hz);
void Network_Poll(void);

#ifdef __cplusplus
}
#endif

#endif /* __FMSTR_NETWORK_H */
