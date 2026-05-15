/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __IPC_SHM__CLIENT__
#define __IPC_SHM__CLIENT__
#include <ipc_shm.h>

extern struct ipc_shm_t *ipc_shm;

inline void ipc_shm_client_sync()
{
	while (ipc_shm->client_sync == ipc_shm->host_sync);
	ipc_shm->client_sync++;
}

inline void ipc_shm_client_sync_inc()
{
	ipc_shm->client_sync++;
}

inline int ipc_shm_client_sync_check()
{
	return ipc_shm->host_sync - ipc_shm->client_sync > 1 ? -1 : 0;
}

int ipc_shm_client_find_channel(char *name, enum channel_type type, enum channel_direction dir);
int ipc_shm_duplex_channel_find(struct duplex_channel *channel, char *name, enum channel_type type);
int ipc_shm_client_sdc_find(struct duplex_channel *channel, char *name);
int ipc_shm_client_pdc_find(struct duplex_channel *channel, char *name);
int ipc_shm_client_sdc_read(struct duplex_channel *sdc, void *data, uint32_t size);
int ipc_shm_client_sdc_write(struct duplex_channel *sdc, void *data, uint32_t size);
void *ipc_shm_get_param_pointer(int handler);
void *ipc_shm_client_pdc_read_start(struct duplex_channel *pdc);
void ipc_shm_client_pdc_read_end(struct duplex_channel *pdc);
void *ipc_shm_client_pdc_write_start(struct duplex_channel *pdc);
void ipc_shm_client_pdc_write_end(struct duplex_channel *pdc);
void *ipc_shm_param_find(char *name);
#endif
