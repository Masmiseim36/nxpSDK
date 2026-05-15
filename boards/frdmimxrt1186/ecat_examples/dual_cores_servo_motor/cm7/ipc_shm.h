/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __IPC__SHM___
#define __IPC__SHM___
#include <inttypes.h>
#include <stddef.h>
#define MAX_CHANNEL_NAME 10
#define BUFFER_MEM_NUMBER 3
#define MAILBOX_MEM_NUMBER 3
#define MAILBOX_MEM_PARAM 1
#define MAX_CHANNEL_NUM 10
extern struct ipc_shm_t *ipc_shm;
enum mem_entry_status {
	mem_entry_idle = 0,
	mem_entry_writing,
	mem_entry_full,
	mem_entry_reading
}  __attribute__ ((packed));

enum channel_direction {
	channel_host_to_clinet = 0,
	channel_client_to_host,
	channel_duplex
}  __attribute__ ((packed));

// Inter Process Communication
enum channel_type {
	channel_type_buffer,
	channel_type_mailbox,
	channel_type_param
}  __attribute__ ((packed));

struct channel_metadata {
	enum mem_entry_status status[3];
	void *mem_entry[3];
	uint32_t mem_data_len[3];
	uint32_t mem_entry_size;
	char channel_name[MAX_CHANNEL_NAME];
	uint8_t reading_index;
	uint8_t writing_index;
	enum channel_type type;
	enum channel_direction dir;
	uint32_t user_data;
} __attribute__ ((aligned (4)));

struct ipc_shm_t
{
	volatile uint32_t host_sync;
	volatile uint32_t client_sync;
	void *data_base;
	void *data_top;
	void *data_heap;
	uint32_t size;
	uint32_t channel_index;
	struct channel_metadata metadata[MAX_CHANNEL_NUM];
} __attribute__ ((aligned (4)));

struct duplex_channel
{
	uint32_t size_to_client;
	uint32_t size_to_host;
	int handler_w;
	int handler_r;
	int start_tick;
	char name[MAX_CHANNEL_NAME];
};

void *ipc_shm_mailbox_write_start(int handler);
int ipc_shm_mailbox_write_end(int handler, uint32_t len);
void *ipc_shm_mailbox_read_start(int handler, uint32_t *len);
int ipc_shm_mailbox_read_end(int handler);
void *ipc_shm_buffer_write_start(int handler);
void ipc_shm_buffer_write_end(int handler, uint32_t len);
void *ipc_shm_buffer_read_start(int handler, uint32_t *len);
void ipc_shm_buffer_read_end(int handler);
#endif
