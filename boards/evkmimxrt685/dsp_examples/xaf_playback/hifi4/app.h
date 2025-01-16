/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "dsp_config.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_RPMSG_READY_EVENT_DATA    (1)
#define APP_RPMSG_EP_READY_EVENT_DATA (2)

#define NOTIF_EVT_RPMSG_RECEIVED_DATA (1 << 0)
#define NOTIF_EVT                     (NOTIF_EVT_RPMSG_RECEIVED_DATA)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
