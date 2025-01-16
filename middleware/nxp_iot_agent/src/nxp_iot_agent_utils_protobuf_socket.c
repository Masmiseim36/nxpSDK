/* 
 * Copyright 2018-2021,2023-2024 NXP
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <nxp_iot_agent_utils_protobuf.h>
#include <nxp_iot_agent_utils_internal.h>
#include <nxp_iot_agent_log.h>
#include <network.h>

#if defined(ENABLE_IOT_AGENT_PROTOBUF_DEBUG) && (ENABLE_IOT_AGENT_PROTOBUF_DEBUG == 1)
void print_binary_data(const char* info, const uint8_t* buf, size_t len) {
	size_t i;
	printf("%-20s length: %d\n        ", info, len);
	for (i = 0; i < len;) {
		printf("%02x", buf[i]);
		++i;

		if ((i % 16) == 0) {
			printf("\n        ");
		}
	}
	printf("\n");
}
#endif

static bool write_callback(pb_ostream_t *stream, const pb_byte_t *buf, size_t count)
{
	void* network_context = stream->state;

#if defined(ENABLE_IOT_AGENT_PROTOBUF_DEBUG) && (ENABLE_IOT_AGENT_PROTOBUF_DEBUG == 1)
	print_binary_data("write", buf, count);
#endif

	int32_t written = network_write(network_context, buf, count);
	if (written < 0)
	{
		IOT_AGENT_ERROR("Error in network write");
		return false;
	}
	return ((size_t)written == count);
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
	void* network_context = stream->state;
#if defined(ENABLE_IOT_AGENT_PROTOBUF_DEBUG) && (ENABLE_IOT_AGENT_PROTOBUF_DEBUG == 1)
	uint8_t* start = buf;
	int total = 0;
#endif
	while (count > 0U)
	{
		int read = network_read(network_context, buf, count);
		if (read <= 0)
		{
			stream->bytes_left = 0U; /* EOF */
			break;
		}
		buf += read;
		count -= (size_t)read;
#if defined(ENABLE_IOT_AGENT_PROTOBUF_DEBUG) && (ENABLE_IOT_AGENT_PROTOBUF_DEBUG == 1)
		total += read;
#endif
	}

#if defined(ENABLE_IOT_AGENT_PROTOBUF_DEBUG) && (ENABLE_IOT_AGENT_PROTOBUF_DEBUG == 1)
	if (total > 0)
		print_binary_data("read", start, total);
#endif
	return (count == 0U);
}

pb_ostream_t ostream_from_socket(void* network_context)
{
	pb_ostream_t stream = { &write_callback, network_context, SIZE_MAX, 0U };
	return stream;
}

pb_istream_t istream_from_socket(void* network_context)
{
	pb_istream_t stream = { &read_callback, network_context, SIZE_MAX };
	return stream;
}

