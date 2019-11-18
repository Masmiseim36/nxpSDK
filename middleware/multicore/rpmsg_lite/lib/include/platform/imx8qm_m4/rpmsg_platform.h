/*
 * Copyright (c) 2017 NXP Semiconductor, Inc.
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RPMSG_PLATFORM_H
#define _RPMSG_PLATFORM_H

/* RPMSG MU channel index */
#define RPMSG_MU_CHANNEL (1)

/*
 * Linux requires the ALIGN to 0x1000(4KB) instead of 0x80
 */
#ifndef VRING_ALIGN
#define VRING_ALIGN (0x1000)
#endif

/* contains pool of descriptors and two circular buffers */
#ifndef VRING_SIZE
#define VRING_SIZE (0x8000)
#endif

/* size of shared memory + 2*VRING size */
#define RL_VRING_OVERHEAD (2 * VRING_SIZE)

/* VQ_ID in 8QM is defined as follows:
 *   com_id:   [3:3] communication ID, used to identify the MU instance.
 *   vring_id: [2:1] vring ID, used to identify the vring.
 *   q_id:     [0:0] queue ID, used to identify the tvq or rvq.
 *   com_id + vring_id = link_id
 */

#define RL_GET_VQ_ID(link_id, queue_id) (((queue_id) & 0x1) | (((link_id) << 1) & 0xFFFFFFFE))
#define RL_GET_LINK_ID(vq_id) ((vq_id) >> 1)
#define RL_GET_COM_ID(vq_id) ((vq_id) >> 3)
#define RL_GET_Q_ID(vq_id) ((vq_id) & 0x1)

#define RL_GEN_LINK_ID(com, vring) (((com) << 2) | (vring))
#define RL_GEN_MU_MSG(vq_id) (((uint32_t)vq_id & 0x7) << 16) /* com_id is discarded in msg */

#define RL_PLATFORM_IMX8QM_M4_A_COM_ID (0)
#define RL_PLATFORM_IMX8QM_M4_M4_COM_ID (1)

#define RL_PLATFORM_IMX8QM_M4_SRTM_VRING_ID (0)
#define RL_PLATFORM_IMX8QM_M4_USER_VRING_ID (1)

#define RL_PLATFORM_HIGHEST_LINK_ID RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_M4_COM_ID, RL_PLATFORM_IMX8QM_M4_USER_VRING_ID)

#define RL_PLATFORM_IMX8QM_M4_A_SRTM_LINK_ID \
              RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_A_COM_ID, RL_PLATFORM_IMX8QM_M4_SRTM_VRING_ID)
#define RL_PLATFORM_IMX8QM_M4_A_USER_LINK_ID \
              RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_A_COM_ID, RL_PLATFORM_IMX8QM_M4_USER_VRING_ID)
#define RL_PLATFORM_IMX8QM_M4_M4_SRTM_LINK_ID \
              RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_M4_COM_ID, RL_PLATFORM_IMX8QM_M4_SRTM_VRING_ID)
#define RL_PLATFORM_IMX8QM_M4_M4_USER_LINK_ID \
              RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_M4_COM_ID, RL_PLATFORM_IMX8QM_M4_USER_VRING_ID)

/* platform interrupt related functions */
int platform_init_interrupt(int vector_id, void *isr_data);
int platform_deinit_interrupt(int vector_id);
int platform_interrupt_enable(unsigned int vector_id);
int platform_interrupt_disable(unsigned int vector_id);
int platform_in_isr(void);
void platform_notify(int vector_id);

/* platform low-level time-delay (busy loop) */
void platform_time_delay(int num_msec);

/* platform memory functions */
void platform_map_mem_region(unsigned int va, unsigned int pa, unsigned int size, unsigned int flags);
void platform_cache_all_flush_invalidate(void);
void platform_cache_disable(void);
unsigned long platform_vatopa(void *addr);
void *platform_patova(unsigned long addr);

/* platform init/deinit */
int platform_init(void);
int platform_deinit(void);

#endif /* _RPMSG_PLATFORM_H */
