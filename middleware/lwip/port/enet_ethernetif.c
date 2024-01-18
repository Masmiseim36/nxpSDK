/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020,2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
// #include "fsl_enet.h"
#include "ethernetif_priv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void ethernetif_enet_init(struct netif *netif,
                          struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig);

void *ethernetif_get_enet_base(const uint8_t enetIdx)
{
    ENET_Type *enets[] = ENET_BASE_PTRS;
    int arrayIdx;
    int enetCount;

    for (arrayIdx = 0, enetCount = 0; arrayIdx < ARRAY_SIZE(enets); arrayIdx++)
    {
        if (enets[arrayIdx] != 0U) /* process only defined positions */
        {                          /* (some SOC headers count ENETs from 1 instead of 0) */
            if (enetCount == enetIdx)
            {
                return (void *)enets[arrayIdx];
            }
            enetCount++;
        }
    }

    return NULL;
}

#if defined(FSL_FEATURE_SOC_ENET_QOS_COUNT) && (FSL_FEATURE_SOC_ENET_QOS_COUNT > 0)
void *ethernetif_get_enet_qos_base(const uint8_t enetIdx)
{
    ENET_QOS_Type *enets[] = ENET_QOS_BASE_PTRS;
    int arrayIdx;
    int enetCount;

    for (arrayIdx = 0, enetCount = 0; arrayIdx < ARRAY_SIZE(enets); arrayIdx++)
    {
        if (enets[arrayIdx] != 0U) /* process only defined positions */
        {                          /* (some SOC headers count ENETs from 1 instead of 0) */
            if (enetCount == enetIdx)
            {
                return (void *)enets[arrayIdx];
            }
            enetCount++;
        }
    }

    return NULL;
}

#endif
