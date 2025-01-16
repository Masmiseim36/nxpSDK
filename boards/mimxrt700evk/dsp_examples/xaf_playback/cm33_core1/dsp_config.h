/*
 * Copyright 2021-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DSP_CONFIG_H_
#define _DSP_CONFIG_H_

#include "fsl_device_registers.h"

#define DSP_EPT_ADDR (30)
#define MCU_EPT_ADDR (40)

/* Address of HiFi4 RAM, where the dsp image should be copied */
#define DSP_LITERAL_ADDRESS (uint32_t *)0x24000000
#define DSP_BOOT_ADDRESS    (uint32_t *)0x24020000
#define DSP_SRAM_ADDRESS    (uint32_t *)0x20300000
/* Address of HiFi1 RAM, where the dsp image should be copied */
#define HIFI1_BOOT_ADDRESS (uint32_t *)0x580000
#define HIFI1_TEXT_ADDRESS (uint32_t *)0x740000
#define HIFI1_DATA_ADDRESS (uint32_t *)0x20680000

/* Inter processor communication common RAM */
#if (defined(MIMXRT798S_cm33_core0_SERIES) || defined(MIMXRT798S_hifi4_SERIES))
#define RPMSG_LITE_LINK_ID    (RL_PLATFORM_IMXRT700_M33_0_HIFI4_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE (void *)0x20200000
/* Audio ping/pong buffers for streaming DSP handling.
 * Assign each buffer to distinct shared RAM partition to maximize performance. */
#define AUDIO_SHARED_BUFFER_1      0x20020000
#define AUDIO_SHARED_BUFFER_1_SIZE (128 * 1024)
#define AUDIO_SHARED_BUFFER_2      0x20040000
#define AUDIO_SHARED_BUFFER_2_SIZE (128 * 1024)
/* DSP-private uncached audio memory buffers */
/* Each buffer is 32k in size in a separate SRAM partition */
#define DSP_AUDIO_BUFFER_1_PING (0x20000000)
#define DSP_AUDIO_BUFFER_1_PONG (0x20008000)
#define DSP_AUDIO_BUFFER_2_PING (0x20010000)
#define DSP_AUDIO_BUFFER_2_PONG (0x20018000)
#else
#define RPMSG_LITE_LINK_ID    (RL_PLATFORM_IMXRT700_M33_1_HIFI1_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE (void *)0x20588000
/* Audio ping/pong buffers for streaming DSP handling.
 * Assign each buffer to distinct shared RAM partition to maximize performance. */
#define AUDIO_SHARED_BUFFER_1      0x20590000
#define AUDIO_SHARED_BUFFER_1_SIZE (64 * 1024)
#define AUDIO_SHARED_BUFFER_2      0x205a0000
#define AUDIO_SHARED_BUFFER_2_SIZE (64 * 1024)
/* DSP-private audio memory buffers */
/* Each buffer is 32k in size in a separate SRAM partition */
#define DSP_AUDIO_BUFFER_1_PING (0x205b0000)
#define DSP_AUDIO_BUFFER_1_PONG (0x205b8000)
#endif
#define RPMSG_LITE_SHMEM_SIZE (8 * 1024)

#define AUDIO_I2S_RENDERER_DEVICE 0

#endif
