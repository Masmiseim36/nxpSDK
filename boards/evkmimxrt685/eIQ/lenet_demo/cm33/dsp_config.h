/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DSP_CONFIG_H_
#define _DSP_CONFIG_H_

#define DSP_EPT_ADDR (30)
#define MCU_EPT_ADDR (40)

/* Address of RAM, where the image for dsp should be copied */
#define DSP_BOOT_ADDRESS (uint32_t *)0x24000000
#define DSP_SRAM_ADDRESS (uint32_t *)0x00300000

/* Inter processor communication common RAM */
#define RPMSG_LITE_LINK_ID    (RL_PLATFORM_IMXRT600_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE (void *)0x20000000
#define RPMSG_LITE_SHMEM_SIZE (0x10000)

#endif
