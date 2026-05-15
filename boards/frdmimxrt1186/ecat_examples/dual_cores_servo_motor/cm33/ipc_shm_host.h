/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __IPC_SHM__HOST__
#define __IPC_SHM__HOST__
#include <ipc_shm.h>
extern struct ipc_shm_t *ipc_shm;
inline void ipc_shm_host_sync_updata()
{
	ipc_shm->host_sync++;
}

int ipc_shm_host_init(uint32_t ipc_shm_base, uint32_t ipc_shm_size);
int ipc_shm_host_alloc_channel(enum channel_type type, uint32_t data_size, char *channel_name, enum channel_direction dir);
int ipc_shm_duplex_channel_create(char *name, uint32_t size_w, uint32_t size_r, struct duplex_channel *channel, enum channel_type type);
int ipc_shm_sdc_create(char *name, uint32_t size_w, uint32_t size_r, struct duplex_channel *sdc);
int ipc_shm_host_sdc_send(struct duplex_channel *sdc, void *data, uint32_t size);
int ipc_shm_host_sdc_recv(struct duplex_channel *sdc, void *data, uint32_t size);
int ipc_shm_param_create(char *name, uint32_t size);
int ipc_shm_pdc_create(char *name, uint32_t size_w, uint32_t size_r, struct duplex_channel *pdc);
void *ipc_shm_host_pdc_send_pre(struct duplex_channel *pdc);
void ipc_shm_host_pdc_send(struct duplex_channel *pdc);
void *ipc_shm_host_pdc_recv(struct duplex_channel *pdc);
void ipc_shm_host_pdc_recv_post(struct duplex_channel *pdc);
void *ipc_shm_get_param_pointer(int handler);
#endif
