/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization to use this header is provided through a DeepViewRT license.
 *
 * @file deepview_rpc.h DeepViewRT ModelRunner UNIX RPC
 *
 * Low-level RPC Request for UNIX Domain Sockets.  These are defined assuming
 * DGRAM framing and the payload is simply appended after the request/response
 * messages.  The sizing of the payload is controlled by the DGRAM framing.
 *
 * The request can include frame data in one of three ways, in all cases the
 * fourcc code should be set appropriately.
 *
 * 1. Frame data contained in payload, the frame data starts immediately
 *    following the rpc_request header (must be unstrided).  Frame size limits
 *    apply, generally only viable for smaller encoded images as PNG or JPEG.
 * 2. Frame data contained in physical memory address rpc_request::memory.
 * 3. Frame data contained in previously mapped shared memory and with
 *    optional offset from start of shared memory stored in rpc_request::memory.
 *
 * To use shared memory a single mapping can be defined with the
 * rpc_command::RPC_MAP_SHARED_MEMORY command message in which case the payload
 * should be the path to shared memory. Though only a single shared memory
 * object can be mapped at a time, multi-buffering can be supported through the
 * use of offsets into the shared memory allowing multiple frames to be packed
 * into a single shared memory object.  In these cases the rpc_request::memory
 * field is interpreted as the offset into the shared memory mapping.
 */

#ifndef __DEEPVIEW_RPC_H__
#define __DEEPVIEW_RPC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * RPC Request header structure, all requests must provide at least this header.
 * Any additional data is considered the payload.
 */
struct rpc_request {
    /**
     * RPC Command field, refer to @ref enum rpc_command for available commands.
     */
    uint16_t command;
    /**
     * RPC Control field, only used when command is RPC_RUN_MODEL and controls
     * the run options such as loading input data and output handling.
     */
    uint16_t control;
    /**
     * The width of the frame provided by the control RPC_LOAD_INPUT, used for
     * shared memory and DMA frames.  Data loaded through RPC_DECODE_INPUT would
     * typically not require width/height to be provided as the CODEC provides
     * this information.
     */
    uint16_t width;
    /**
     * The height of the frame provided by the control RPC_LOAD_INPUT.  Refer
     * to @ref rpc_request::width for details.
     */
    uint16_t height;
    /**
     * Region of interest for processing.  After decoding/converting the input
     * frame a crop will be taken then scaled to fit into the model input. If
     * width and/or height are 0 then roi processing is ignored.
     */
    struct roi {
        /**
         * Width of the ROI.
         */
        uint16_t width;
        /**
         * Height of the ROI.
         */
        uint16_t height;
        /**
         * Horizontal offset from the left of the frame for the ROI.
         */
        uint16_t x;
        /**
         * Vertical offset from the top of the frame for the ROI.
         */
        uint16_t y;
    } roi;
    /**
     * The FOURCC code for the video frame, including extensions such as 'PNG '
     * and 'JPEG'.  This is typically used for direct video frames in DMA or
     * shared memory and will contain a RGB or YUV pixel definition.
     */
    uint32_t fourcc;
    /**
     * The offset address for the frame memory.  For DMA this is the absolute
     * physical address of the video memory for the current frame.  For shared
     * memory this is the offset into the shared memory object.
     */
    uint64_t memory;
};

/**
 * RPC Command bitmask for the @ref rpc_request::command field.
 */
enum rpc_command {
    /**
     * Map shared memory object, this will be used for shared memory frames in
     * future messages.
     */
    RPC_MAP_SHARED_MEMORY = 1,
    /**
     * Unload the active shared memory object.
     */
    RPC_UNMAP_SHARED_MEMORY,
    /**
     * Load the model from the filesystem using memory map, if a model is
     * already loaded it will first be unloaded.  Note ModelRunner currently
     * only supports  a single active model, though multiple ModelRunner
     * instances can be launched to handle multiple models in parallel.
     */
    RPC_LOAD_MODEL,
    /**
     * Unload the model and release the memory map.
     */
    RPC_UNLOAD_MODEL,
    /**
     * Sets the name of the input layer, by default this is "input".
     */
    RPC_SET_INPUT_NAME,
    /**
     * Sets the name of the output layer, by default this is "output".
     */
    RPC_SET_OUTPUT_NAME,
    /**
     * Runs the model.  The control field can be used to load input data and
     * request results when running the model, but these are optional and run
     * can be called without consideration for input/output handling.
     */
    RPC_RUN_MODEL,
};

/**
 * RPC Control bitmask for the @ref rpc_request::control field.
 */
enum rpc_control {
    RPC_LOAD_INPUT        = 0x0001,
    RPC_DECODE_INPUT      = 0x0002,
    RPC_MIRROR_INPUT      = 0x0004,
    RPC_FLIP_INPUT        = 0x0008,
    RPC_RETURN_INDEX      = 0x0010,
    RPC_RETURN_SCORE      = 0x0040,
    RPC_RETURN_LABEL      = 0x0020,
    RPC_RETURN_OUTPUT     = 0x0080,
    RPC_CLEAR_TENSOR      = 0x0100,
    RPC_INPUT_NORMALIZE   = 0x1000,
    RPC_INPUT_WHITENING   = 0x2000,
    RPC_INPUT_SIGNED_NORM = 0x4000
};

/**
 * RPC Response header which will always be included as a minimum in ModelRunner
 * responses, additional data is considered the payload.
 *
 * If  rpc_control::RPC_RETURN_OUTPUT was set the response will include the
 * output tensor's data in the payload, rpc_status::RPC_PAYLOAD_OUTPUT will be
 * set and rpc_response::n_dims and rpc_response::output_shape will provide a
 * valid representation of the tensor's shape.
 *
 * The UNIX RPC can only provide output for a single layer, for more fine
 * grained output selection the higher level HTTP REST API is required.  The
 * output layer by default is the layer named "output", this can be controlled
 * through the rpc_command::RPC_SET_OUTPUT_NAME command message.
 */
struct rpc_response {
    /**
     * Model evaluation time in nanoseconds.  Detailed profiling information,
     * including frame load/decode timing, is available through the HTTP REST
     * API.
     */
    int64_t timing;
    /**
     * RPC Response status mask, see @ref rpc_status for details.
     */
    int32_t status;
    /**
     * If rpc_status::RPC_PAYLOAD_OUTPUT is set this value is set the the number
     * of valid dimensions in the rpc_response::output_shape field.
     */
    int32_t n_dims;
    /**
     * When rpc_status::RPC_PAYLOAD_OUTPUT is set this field provides the shape
     * of the model's output layer.  All 4 dimensions will always be set but
     * only the first @rpc_response::n_dims should be interpreted, the remaining
     * dimensions will always be 1.
     */
    int32_t output_shape[4];
};

/**
 * RPC Status bitmask.  The payload data can all be included in a given message
 * and so each should be tested when interpreting the output.  The ordering of
 * the payload follows their integer values (message, index, score, label,
 * output).
 *
 * Except for output, all the payloads are NULL-terminated strings so parsing is
 * done by simply reading the const char* value of the payload.  Chaining
 * payloads is done by moving the pointer of the next payload to strlen(payload)
 * + 1.  As messages are transmitted as datagrams we always know the full frame
 * size and use this to calculate the total size of the payload.
 */
enum rpc_status {
    /**
     * RPC_SUCCESS doesn't actually set a bit but it is the alternate of the
     * RPC_ERROR status.  It is possible for informational messages to be
     * included in the payload, test with RPC_PAYLOAD_MESSAGE.
     */
    RPC_SUCCESS = 0x00000000,
    /**
     * RPC_ERROR signals an error occurred during processing, typically the
     * error message will be stored in the payload but RPC_PAYLOAD_MESSAGE needs
     * to be tested to confirm.
     */
    RPC_ERROR = 0x00000001,
    /**
     * RPC_PAYLOAD_MESSAGE signals that there is a message stored in the payload
     * such as an error message or informational output.
     */
    RPC_PAYLOAD_MESSAGE = 0x00000002,
    RPC_PAYLOAD_INDEX   = 0x00000010,
    RPC_PAYLOAD_SCORE   = 0x00000020,
    RPC_PAYLOAD_LABEL   = 0x00000040,
    RPC_PAYLOAD_OUTPUT  = 0x00000080,
};

#ifdef __cplusplus
}
#endif

#endif /* __DEEPVIEW_RPC_H__ */