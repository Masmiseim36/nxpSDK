/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "ipc_shm_host.h"

#ifdef HOST_LINUX
#include "ipc_shm_mmap.h"
#else
#include "ipc_shm_mem.h"
#endif
struct ipc_shm_t *ipc_shm;
int ipc_shm_host_init(uint32_t ipc_shm_base, uint32_t ipc_shm_size)
{
	struct ipc_shm_mem ipc_shm_mem = {0};
	if (ipc_shm_memmap(ipc_shm_base, ipc_shm_size, &ipc_shm_mem) < 0) {
		return -1;
	}

	ipc_shm = (struct ipc_shm_t *)ipc_shm_mem.base;
	ipc_shm->data_base = (void *)((char *)ipc_shm_mem.base + sizeof(struct ipc_shm_t));
	ipc_shm->data_top = (void *)((char *)ipc_shm_mem.base + ipc_shm_mem.size);
	ipc_shm->data_heap = (void *)((char *)ipc_shm_mem.base + ipc_shm_mem.size);
	ipc_shm->size = ipc_shm_mem.size;
	ipc_shm->channel_index = 0;	
	ipc_shm->host_sync = 0;
	ipc_shm->client_sync = 0;
	return 0;
}

int ipc_shm_host_alloc_channel(enum channel_type type, uint32_t data_size, char *channel_name, enum channel_direction dir)
{
	struct channel_metadata *meta;
	uint8_t *data;
	int handler, i, mem_number;

	if (ipc_shm->channel_index >= MAX_CHANNEL_NUM) {
		return -1;
	}

	if (type == channel_type_buffer)
		mem_number = BUFFER_MEM_NUMBER;
	else if (type == channel_type_mailbox)
		mem_number = MAILBOX_MEM_NUMBER;
	else if (type == channel_type_param)
		mem_number = MAILBOX_MEM_PARAM;
	else
		mem_number = BUFFER_MEM_NUMBER;

	meta = &ipc_shm->metadata[ipc_shm->channel_index];
	data = (uint8_t *)ipc_shm->data_heap - data_size * mem_number;
	if ((uint32_t)data < (uint32_t)ipc_shm->data_base) {
		return -2;
	}
	ipc_shm->data_heap = data;

	for (i = 0; i < mem_number; i++) {
		meta->mem_entry[i] = data + i * data_size;
		meta->status[i] = mem_entry_idle;
		meta->mem_data_len[i] = 0;
	}
	strncpy(meta->channel_name, channel_name, MAX_CHANNEL_NAME);
	meta->type = type;
	meta->reading_index = 0;
	meta->writing_index = 0;
	meta->dir = dir;
	meta->mem_entry_size = data_size;
	handler = ipc_shm->channel_index;
	ipc_shm->channel_index++;
	return handler;
}

int ipc_shm_duplex_channel_create(char *name, uint32_t size_w, uint32_t size_r, struct duplex_channel *channel, enum channel_type type)
{
	int handler;
	if ((handler = ipc_shm_host_alloc_channel(type, size_w, name, channel_host_to_clinet)) < 0) {
		return -1;
	}
	channel->handler_w = handler;
	
	if ((handler = ipc_shm_host_alloc_channel(type, size_r, name, channel_client_to_host)) < 0) {
		return -1;
	}
	channel->handler_r = handler;
	channel->size_to_client = size_w;
	channel->size_to_host = size_r;
	return 0;
}

// Create a paramers share memory

int ipc_shm_param_create(char *name, uint32_t size)
{
	return ipc_shm_host_alloc_channel(channel_type_param, size, name, channel_duplex);
}

// Create a new server data channel
int ipc_shm_sdc_create(char *name, uint32_t size_w, uint32_t size_r, struct duplex_channel *sdc)
{
	return ipc_shm_duplex_channel_create(name, size_w, size_r, sdc, channel_type_mailbox);
}

int ipc_shm_host_sdc_send(struct duplex_channel *sdc, void *data, uint32_t size)
{
	uint32_t len;
	uint32_t index = 0;
	uint8_t *mem;
	while (size) {
		len = size < sdc->size_to_client ? size : sdc->size_to_client;
		do {
			mem = ipc_shm_mailbox_write_start(sdc->handler_w);
		} while (!mem);
		memcpy(mem, (char *)data + index, len);
		index += len;
		size -= len;
		ipc_shm_mailbox_write_end(sdc->handler_w, len);
	}
	return 0;
}

int ipc_shm_host_sdc_recv(struct duplex_channel *sdc, void *data, uint32_t size)
{
	uint32_t len;
	uint8_t *mem;

	if ((mem = ipc_shm_mailbox_read_start(sdc->handler_r, &len)) == NULL) {
		return 0;
	}

	if (len > size) {
		return -1;
	}

	memcpy(data, mem, len);
	ipc_shm_mailbox_read_end(sdc->handler_r);
	return len;
}

// Create a new process data channel
int ipc_shm_pdc_create(char *name, uint32_t size_w, uint32_t size_r, struct duplex_channel *pdc)
{
	int i;
	struct channel_metadata *meta;

	if (ipc_shm_duplex_channel_create(name, size_w, size_r, pdc, channel_type_buffer) < 0) {
		return -1;
	}

	/*For process communication, the length of data package is fixed for each cycle*/
	meta = &ipc_shm->metadata[pdc->handler_w];
	for (i = 0; i < BUFFER_MEM_NUMBER; i++) {
		meta->mem_data_len[i] = size_w;
	}
	
	meta = &ipc_shm->metadata[pdc->handler_r];
	for (i = 0; i < BUFFER_MEM_NUMBER; i++) {
		meta->mem_data_len[i] = size_r;
	}
	return 0;
}

void *ipc_shm_host_pdc_send_pre(struct duplex_channel *pdc)
{
	return ipc_shm_buffer_write_start(pdc->handler_w);
}

void ipc_shm_host_pdc_send(struct duplex_channel *pdc)
{
	ipc_shm_buffer_write_end(pdc->handler_w, pdc->size_to_client);
}

void *ipc_shm_host_pdc_recv(struct duplex_channel *pdc)
{
    return ipc_shm_buffer_read_start(pdc->handler_r, NULL);
}

void ipc_shm_host_pdc_recv_post(struct duplex_channel *pdc)
{
    ipc_shm_buffer_read_end(pdc->handler_r);
}
