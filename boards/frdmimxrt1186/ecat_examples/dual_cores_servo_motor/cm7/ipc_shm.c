/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "ipc_shm.h"

void *ipc_shm_get_param_pointer(int handler)
{
	struct channel_metadata *meta;
	meta = &ipc_shm->metadata[handler];
	return meta->mem_entry[0];
}

void *ipc_shm_mailbox_write_start(int handler)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	if (meta->status[meta->writing_index] != mem_entry_idle) {
		return NULL;
	}
	meta->status[meta->writing_index] = mem_entry_writing;
	return meta->mem_entry[meta->writing_index];
}

int ipc_shm_mailbox_write_end(int handler, uint32_t len)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	if (meta->status[meta->writing_index] != mem_entry_writing) {
		return -1;
	}
	meta->mem_data_len[meta->writing_index] = len;
	meta->status[meta->writing_index] = mem_entry_full;

	meta->writing_index++;
	meta->writing_index %= MAILBOX_MEM_NUMBER;
	return 1;
}

void *ipc_shm_mailbox_read_start(int handler, uint32_t *len)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	if (meta->status[meta->reading_index] != mem_entry_full) {
		return NULL;
	}
	meta->status[meta->reading_index] = mem_entry_reading;
	*len = meta->mem_data_len[meta->reading_index];
	return meta->mem_entry[meta->reading_index];
}

int ipc_shm_mailbox_read_end(int handler)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	if (meta->status[meta->reading_index] != mem_entry_reading) {
		return -1;
	}
	meta->status[meta->reading_index] = mem_entry_idle;
	meta->reading_index++;
	meta->reading_index %= MAILBOX_MEM_NUMBER;
	return 1;
}

void *ipc_shm_buffer_write_start(int handler)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	return meta->mem_entry[meta->writing_index];
}

void ipc_shm_buffer_write_end(int handler, uint32_t len)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	meta->mem_data_len[meta->writing_index] = len; 
	meta->writing_index++;
	meta->writing_index %= BUFFER_MEM_NUMBER;
}

void *ipc_shm_buffer_read_start(int handler, uint32_t *len)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	if (len)
		*len = meta->mem_data_len[meta->reading_index];
	return meta->mem_entry[meta->reading_index];
}

void ipc_shm_buffer_read_end(int handler)
{
	struct channel_metadata *meta;

	meta = &ipc_shm->metadata[handler];
	meta->reading_index++;
	meta->reading_index %= BUFFER_MEM_NUMBER;
}
