/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "ipc_shm_client.h"

int ipc_shm_client_find_channel(char *name, enum channel_type type, enum channel_direction dir)
{
	struct channel_metadata *meta;
	int i;

	for (i = 0; i < ipc_shm->channel_index; i++) {
		meta = &ipc_shm->metadata[i];
		if (strncmp(name, meta->channel_name, MAX_CHANNEL_NAME) == 0 && meta->type == type && meta->dir == dir) {
			return i;
		}
	}

	return -1;
}

int ipc_shm_duplex_channel_find(struct duplex_channel *channel, char *name, enum channel_type type)
{
	int handler;

	if ((handler = ipc_shm_client_find_channel(name, type, channel_host_to_clinet)) < 0) {
		return -1;
	}
	channel->handler_w = handler;

	if ((handler = ipc_shm_client_find_channel(name, type, channel_client_to_host)) < 0) {
		return -2;
	}
	channel->handler_r = handler;

	channel->size_to_client = ipc_shm->metadata[channel->handler_w].mem_entry_size;
	channel->size_to_host = ipc_shm->metadata[channel->handler_r].mem_entry_size;
	strncpy(channel->name, name, MAX_CHANNEL_NAME);
	return 0;
}

void* ipc_shm_param_find(char *name)
{
	int handler;
	if ((handler = ipc_shm_client_find_channel(name, channel_type_param, channel_duplex)) < 0) {
        return NULL;
    }

    return ipc_shm->metadata[handler].mem_entry[0];
}


int ipc_shm_client_sdc_find(struct duplex_channel *channel, char *name)
{
	return ipc_shm_duplex_channel_find(channel, name, channel_type_mailbox);
}

int ipc_shm_client_pdc_find(struct duplex_channel *channel, char *name)
{
	return ipc_shm_duplex_channel_find(channel, name, channel_type_buffer);
}

int ipc_shm_client_sdc_read(struct duplex_channel *sdc, void *data, uint32_t size)
{
	uint32_t len;
	uint8_t *mem;

	if ((mem = ipc_shm_mailbox_read_start(sdc->handler_w, &len)) == NULL) {
		return 0;
	}

	if (len > size) {
		return -1;
	}

	memcpy(data, mem, len);
	ipc_shm_mailbox_read_end(sdc->handler_w);
	return len;
}

int ipc_shm_client_sdc_write(struct duplex_channel *sdc, void *data, uint32_t size)
{
	uint32_t len;
	uint32_t index = 0;
	uint8_t *mem;
	while (size) {
		len = size < sdc->size_to_host ? size : sdc->size_to_host;
		do {
			mem = ipc_shm_mailbox_write_start(sdc->handler_r);
		} while (!mem);
		memcpy(mem, (char *)data + index, len);
		index += len;
		size -= len;
		ipc_shm_mailbox_write_end(sdc->handler_r, len);
	}
	return index;
}

void *ipc_shm_client_pdc_read_start(struct duplex_channel *pdc)
{
	return ipc_shm_buffer_read_start(pdc->handler_w, NULL);
}

void ipc_shm_client_pdc_read_end(struct duplex_channel *pdc)
{
	ipc_shm_buffer_read_end(pdc->handler_w);
}

void *ipc_shm_client_pdc_write_start(struct duplex_channel *pdc)
{
    return ipc_shm_buffer_write_start(pdc->handler_r);
}

void ipc_shm_client_pdc_write_end(struct duplex_channel *pdc)
{
    ipc_shm_buffer_write_end(pdc->handler_r, pdc->size_to_client);
}

