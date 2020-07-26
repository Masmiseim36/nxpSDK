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
#include "fsl_sim.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void get_mqtt_id(uint32_t *id)
{
    sim_uid_t uid;
    int i;

    SIM_GetUniqueId(&uid);
    for (i = 0; i < MQTT_ID_SIZE; i++)
    {
        id[i] = 0U;
    }

    i       = 0;
    id[i++] = uid.L;

#if (defined(FSL_FEATURE_SIM_HAS_UIDM) && FSL_FEATURE_SIM_HAS_UIDM)
    id[i++] = uid.M;
#else
    id[i++] = uid.ML;
    id[i++] = uid.MH;
#endif

#if defined(SIM_UIDH)
    id[i++] = uid.H;
#endif
}
