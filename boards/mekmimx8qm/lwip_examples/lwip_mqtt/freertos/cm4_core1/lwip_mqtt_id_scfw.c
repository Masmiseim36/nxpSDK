/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip_mqtt_id.h"

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "misc_api.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if MQTT_ID_SIZE < 2
#error "Size of unique ID is 2 words on this platform but MQTT_ID_SIZE < 2"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void get_mqtt_id(uint32_t *id)
{
    sc_ipc_t ipc;
    int i;

    ipc = SystemGetScfwIpcHandle();
    LWIP_ASSERT("get_mqtt_id(): SCFW IPC has not been initialized\r\n", ipc);

    for (i = 0; i < MQTT_ID_SIZE; i++)
    {
        id[i] = 0U;
    }

    sc_misc_unique_id(ipc, &id[0], &id[1]);
}
