/* 
 * Copyright 2018-2019,2021,2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _NXP_IOT_AGENT_UTILS_PROTOBUF_H_
#define _NXP_IOT_AGENT_UTILS_PROTOBUF_H_

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

/*! @brief Decodes a byte field.
 * @param[in] stream Input stream
 * @param[in] filed Input pb field
 * @param[in] args Optional arguments
 */
bool decode_byte_field(pb_istream_t *stream, const pb_field_t *field, void **arg);

/*! @brief Encodes a byte field.
 * @param[out] stream Output stream
 * @param[in] filed Input pb field
 * @param[in] args arguments
 */
bool encode_byte_field(pb_ostream_t *stream, const pb_field_t *field, void *const *arg);

/*! @brief Encodes an unsigned integer as varint, consuming exactly buffer_size number of bytes.
 *
 * If the number can be encoded using less bytes, the varint is zero-padded to exactly
 * fill the buffer.
 *
 * @param[in] number The number to encode
 * @param[out] buffer The output buffer
 * @param[in] buffer_size Number of bytes
 * @retval True in case of success
 */
bool encode_varint_with_fixed_length(size_t number, uint8_t* buffer, size_t buffer_size);

/*! @brief Decodes expected field.
 * @param[in] stream Input stream
 * @param[in] filed Input pb field
 * @param[in] args Optional arguments
 * @retval True in case of success
 */
bool decode_expect_field(pb_istream_t *stream, const pb_field_t *field, void **arg);

/**
 * @brief Read manually the first tag from the stream and finds the
 * corresponding message type.
 *
 * This function doesn't yet decode the actual message.
 *
 * It returns a pointer to the MsgType_fields array, as an identifier for the
 * message type. Returns null if the tag is of unknown type or an error occurs.
 * @param[in] stream Input stream
 * @param[in] filed Input pb field
 * @retval Returned message type
 */
const pb_field_t* decode_unionmessage_type(pb_istream_t *stream, const pb_field_t* fields);

/*! @brief Verifies the return value.
 * @param[in] stream Input stream
 * @param[in] filed Input pb field
 * @param[in] args Optional arguments
 */
bool verify_return_value(uint16_t rv, expectation_t *expect);

/*! @brief Consumes the bytes in a buffer.
 * @param[in,out] response_buffer Response buffer on which to consume bytes
 * @param[in] len Number of bytes to consume
 */
void pb_response_buffer_consume_bytes(iot_agent_response_buffer_t* response_buffer, size_t len);

/*! @brief Create an ostream that encapsulates a response_buffer.
 * @param[in] response_buffer Response buffer from which to create the ostream.
 * @retval The created ostream
 */
pb_ostream_t pb_ostream_from_response_buffer(iot_agent_response_buffer_t* response_buffer);

// 
/*! @brief Return the internal response_buffer structure encapsulated in an ostream created with pb_ostream_from_response_buffer.
 *
 * @param[in] stream The ostream from which the response buffer will be extracted.
 * @retval Returned response buffer.
 */
iot_agent_response_buffer_t* pb_ostream_response_buffer_get_internal_buffer(pb_ostream_t *stream);


#ifdef __cplusplus
}
#endif

/*!
 *@}
 */ /* end of edgelock2go_agent_utils */

#endif // #ifndef _NXP_IOT_AGENT_UTILS_PROTOBUF_H_
