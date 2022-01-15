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

/*******************************************************************************
 * Code
 ******************************************************************************/

void get_mqtt_id(uint32_t *id)
{
    id[3] = 0U;
    id[2] = 0U;
#ifdef OCOTP_CFG0_BITS_MASK
    id[1] = OCOTP->CFG1;
    id[0] = OCOTP->CFG0;
#else
    id[1] = OCOTP->FUSEN[1].FUSE;
    id[0] = OCOTP->FUSEN[0].FUSE;
#endif
}
