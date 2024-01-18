/*
 * Copyright 2018-2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <nxp_iot_agent_macros.h>
#include <nxp_iot_agent_utils_protobuf.h>
#include <nxp_iot_agent_utils.h>

bool copy_bytes_to_buffer(pb_istream_t *stream, buffer_t *read_buffer)
{
    size_t len = stream->bytes_left;
    if (len > read_buffer->len) {
        IOT_AGENT_ERROR("received byte stream is too long [%d bytes] to "
            "fit in buffer with size [%d bytes]", (int)len, (int)read_buffer->len);
        return false;
    }
    if (!pb_read(stream, (pb_byte_t*)(read_buffer->buf), len)) {
        IOT_AGENT_ERROR("decoding of byte stream failed");
        return false;
    }
    read_buffer->len = len;
    return true;
}


bool decode_byte_field(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    AX_UNUSED_ARG(field);
    buffer_t* read_buffer = (buffer_t*)(*arg);
    return copy_bytes_to_buffer(stream, read_buffer);
}


bool encode_byte_field(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    buffer_t* write_buffer = (buffer_t*)(*arg);
    if (!pb_encode_tag_for_field(stream, field))
    {
        IOT_AGENT_ERROR("pb_encode_tag_for_field failed for encode_byte_field");
        return false;
    }

    if (!pb_encode_string(stream, write_buffer->buf, write_buffer->len))
    {
        IOT_AGENT_ERROR("pb_encode_delimited failed for keystore_info");
        return false;
    }
    return true;
}

bool decode_expect_field(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    AX_UNUSED_ARG(field);
    expectation_t * expectation = (expectation_t*)(*arg);

    uint64_t value;
    if (!pb_decode_varint(stream, &value))
        return false;

    if ( (expectation->offset + sizeof(uint16_t)) > expectation->len ){
        IOT_AGENT_ERROR("received byte stream is too long [%d bytes] to "
            "fit in buffer with size [%d bytes]", (int)(expectation->offset), (int)(expectation->len));
        return false;
    }
    *((uint16_t *)(expectation->buf + expectation->offset)) = (uint16_t)value;
    expectation->offset += sizeof(uint16_t);

    return true;
}

bool verify_return_value(uint16_t rv, expectation_t *expect)
{
    uint16_t expect_value;
    uint16_t * ptr;
    if (expect == NULL)
        return false;

    ptr = (uint16_t *)(expect->buf);
    while((void *)ptr < (void *)(expect->buf + expect->offset)) {
        expect_value = *ptr;
        if (rv == expect_value) {
            IOT_AGENT_DEBUG("Return value is expected");
            return true;
        }
        ptr++;
    }

    IOT_AGENT_ERROR("Return value is unexpected");
    return false;
}

bool encode_varint_with_fixed_length(size_t number, uint8_t* buffer, size_t buffer_size) {
    memset(buffer, 0, buffer_size);
    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, buffer_size);
    if (!pb_encode_varint(&ostream, number))
        return false;

    if (buffer_size > 1U) {
        uint8_t* p = buffer;
        for (size_t i = 0U; i < buffer_size - 1U; i++) {
            *p++ |= 0x80;
        }
    }
    return true;
}

/* This function reads manually the first tag from the stream and finds the
* corresponding message type. It doesn't yet decode the actual message.
*
* Returns a pointer to the MsgType_fields array, as an identifier for the
* message type. Returns null if the tag is of unknown type or an error occurs.
*/
const pb_field_t* decode_unionmessage_type(pb_istream_t *stream, const pb_field_t* fields)
{
    pb_wire_type_t wire_type;
    uint32_t tag;
    bool eof;

    while (pb_decode_tag(stream, &wire_type, &tag, &eof))
    {
        if (wire_type == (pb_wire_type_t)PB_WT_STRING)
        {
            const pb_field_t *field;
            for (field = fields; field->tag != 0U; field++)
            {
                if (field->tag == tag && (field->type & PB_LTYPE_SUBMESSAGE))
                {
                    /* Found our field. */
                    return (const pb_field_t*)field->ptr;
                }
            }
        }

        /* Wasn't our field.. */
        pb_skip_field(stream, wire_type);
    }

    return NULL;
}

pb_ostream_t pb_ostream_from_response_buffer(iot_agent_response_buffer_t* response_buffer)
{
    pb_ostream_t stream = { &pb_ostream_response_buffer_direct_write, response_buffer, response_buffer->remaining, 0U };
    return stream;
}

bool pb_ostream_response_buffer_direct_write(pb_ostream_t *stream, const pb_byte_t *buf, size_t count)
{
    iot_agent_response_buffer_t* response_buffer = (iot_agent_response_buffer_t*)stream->state;
    memcpy(response_buffer->pos, buf, count);
    pb_response_buffer_consume_bytes(response_buffer, count);
    return true;
}

iot_agent_response_buffer_t* pb_ostream_response_buffer_get_internal_buffer(pb_ostream_t *stream)
{
    return (iot_agent_response_buffer_t*)stream->state;
}

void pb_response_buffer_consume_bytes(iot_agent_response_buffer_t* response_buffer, size_t len)
{
    response_buffer->pos += len;
    response_buffer->remaining -= len;
}

int byte_array_to_hex_str(const uint8_t* arr, size_t sz, char* str, size_t strlen)
{
    size_t i;
    char* strp = str;
    char* eos = str + strlen + 1U;

    if (strlen < 1U)
        return 0;

    int wlen = 0;
    for (i = 0U; i < sz; i++)
    {
        /* i use 3 here since we are going to add at most
        2 chars, and need a null terminator */
        if (strp + 3 < eos)
        {
            strp += sprintf(strp, "%02X", arr[i]);
            wlen += 2;
        }
    }
    *strp++ = 0x00;
    return wlen;
}

char* pb_bytes_array_to_hex_str(const pb_bytes_array_t* arr)
{
    size_t len = (size_t)2U * arr->size + 1U;
    char* str = (char*) malloc(len);
    if (str == NULL) return NULL;
    byte_array_to_hex_str(arr->bytes, (size_t)arr->size, str, len);
    return str;
}
