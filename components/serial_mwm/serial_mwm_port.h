/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _SERIAL_MWM_PORT_H_
#define _SERIAL_MWM_PORT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

int mwm_rx(uint8_t *read_buf, uint32_t len);
int mwm_tx(uint8_t *write_buf, uint32_t len);

int mwm_port_init(void);
int mwm_port_deinit(void);

#if defined(__cplusplus)
}
#endif

#endif /* _SERIAL_MWM_PORT_H_ */
