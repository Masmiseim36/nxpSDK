/* 
 * Copyright 2018-2019, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _NXP_IOT_AGENT_UTILS_PROTOBUF_H_
#define _NXP_IOT_AGENT_UTILS_PROTOBUF_H_

#include <nxp_iot_agent_common.h>

#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>

 /*! @defgroup edgelock2go_agent_utils EdgeLock 2GO agent generic util functions.
  * @ingroup edgelock2go_agent
  *
  * @addtogroup edgelock2go_agent_utils
  * @{
  */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t *buf;
	size_t len;
} buffer_t;

typedef struct {
    uint8_t *buf;
    size_t len;
    size_t offset;
} expectation_t;

typedef struct iot_agent_response_buffer_t {
	uint8_t* start;
	uint8_t* pos;
	size_t remaining;
} iot_agent_response_buffer_t;


#if defined(ENABLE_IOT_AGENT_PROTOBUF_DEBUG) && (ENABLE_IOT_AGENT_PROTOBUF_DEBUG == 1)
void print_binary_data(const char* info, const uint8_t* buf, size_t len);
#endif

bool write_callback(pb_ostream_t *stream, const pb_byte_t *buf, size_t count);
bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count);

pb_ostream_t ostream_from_socket(void* network_context);
pb_istream_t istream_from_socket(void* network_context);

// Create an ostream that encapsulates a response_buffer.
pb_ostream_t pb_ostream_from_response_buffer(iot_agent_response_buffer_t* response_buffer);

// On a stream that is encapsulating a response_buffer, direct memory 
// access is possible. This function directly writes to the encapsulated buffer.
bool pb_ostream_response_buffer_direct_write(pb_ostream_t *stream, const pb_byte_t *buf, size_t count);

// Return the internal response_buffer structure encapsulated in an ostream created 
// with pb_ostream_from_response_buffer;
iot_agent_response_buffer_t* pb_ostream_response_buffer_get_internal_buffer(pb_ostream_t *stream);

// In case the response_buffer is directly accessed, this function advances 
// the buffer position by len bytes.
void pb_response_buffer_consume_bytes(iot_agent_response_buffer_t* response_buffer, size_t len);


bool copy_bytes_to_buffer(pb_istream_t *stream, buffer_t *read_buffer);
bool decode_byte_field(pb_istream_t *stream, const pb_field_t *field, void **arg);
bool encode_byte_field(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);
bool decode_expect_field(pb_istream_t *stream, const pb_field_t *field, void **arg);
bool verify_return_value(uint16_t rv, expectation_t *expect);


/* 
 * Encode an unsigned integer as varint, consuming exactly buffer_size number of bytes.
 * If the number can be encoded using less bytes, the varint is zero-padded to exactly 
 * fill the buffer.
 */
bool encode_varint_with_fixed_length(size_t number, uint8_t* buffer, size_t buffer_size);

/**
 * @brief Read manually the first tag from the stream and finds the
 * corresponding message type. 
 *
 * This function doesn't yet decode the actual message.
 *
 * It returns a pointer to the MsgType_fields array, as an identifier for the
 * message type. Returns null if the tag is of unknown type or an error occurs.
 */
const pb_field_t* decode_unionmessage_type(pb_istream_t *stream, const pb_field_t* fields);


/**
 * @brief Convert the contents of a uint8_t array to a hex string.
 *
 * Memory allocation needs to be done by the caller, boundary checks on the output 
 * are performed, null-termination is always added. 
 * @param[in] arr: The binary data to convert.
 * @param[in] sz: The length of the binary data to convert.
 * @param[out] str: The resulting hex string.
 * @param[out] str: Maximum length of characters to write to the resulting 
 *   hex string (incl. the null termination)
 * @return The number of characters written to the hex string (excl. the null termination)
 */
int byte_array_to_hex_str(const uint8_t* arr, size_t sz, char* str, size_t len);


/**
 * @brief Convert the contents of a pb_bytes field to a hex string.
 *
 * The returned hex string is dynamically allocated and ownership is passed to 
 * the caller. It needs to be freed by the caller. 
 */
char* pb_bytes_array_to_hex_str(const pb_bytes_array_t* arr);


#ifdef __cplusplus
}
#endif

/*!
 *@}
 */ /* end of edgelock2go_agent_utils */

#endif // #ifndef _NXP_IOT_AGENT_UTILS_PROTOBUF_H_
