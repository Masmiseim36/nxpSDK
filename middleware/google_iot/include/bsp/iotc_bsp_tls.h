/* Copyright 2018-2019 Google LLC
 *
 * This is part of the Google Cloud IoT Device SDK for Embedded C.
 * It is licensed under the BSD 3-Clause license; you may not use this file
 * except in compliance with the License.
 *
 * You may obtain a copy of the License at:
 *  https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __IOTC_BSP_TLS_H__
#define __IOTC_BSP_TLS_H__

/**
 * @file iotc_bsp_tls.h
 * @brief Implement Transport Layer Security (TLS).
 *
 * Reference implementations are in the <a href="~src/bsp/tls"><code></code></a>
 * directory. All TLS functions are non-blocking operations.
 */

#include <stddef.h>
#include <stdint.h>

/**
 * @typedef iotc_bsp_io_net_state_e
 * @brief TLS function status.
 *
 * TLS function implementations must return a status message to the
 * client application. IOTC_BSP_TLS_STATE_OK represents success and
 * others represent errors.
 */
typedef enum iotc_bsp_tls_state_e {
  /** Operation successful. */
  IOTC_BSP_TLS_STATE_OK = 0,
  /** Can't initialize TLS library. */
  IOTC_BSP_TLS_STATE_INIT_ERROR = 1,
  /** Can't validate CA certificate. */
  IOTC_BSP_TLS_STATE_CERT_ERROR = 2,
  /** Can't complete TLS handshake. */
  IOTC_BSP_TLS_STATE_CONNECT_ERROR = 3,
  /** TLS handshake is partially complete. Run the function again to read the
  remaining data. */
  IOTC_BSP_TLS_STATE_WANT_READ = 4,
  /** TLS handshake is partially complete. Run the function again to write the
  remaining data. */
  IOTC_BSP_TLS_STATE_WANT_WRITE = 5,
  /** Can't read data. */
  IOTC_BSP_TLS_STATE_READ_ERROR = 6,
  /** Can't write data. */
  IOTC_BSP_TLS_STATE_WRITE_ERROR = 7,
} iotc_bsp_tls_state_t;

/**
 * @typedef iotc_bsp_tls_init_params_t
 * @brief Parameter for <code>iotc_bsp_tls_init()</code> to create a TLS
 * context.
 *
 * The memory limiter uses the alloc, calloc, realloc and free functions
 * as part of its memory tracking of the TLS implementation allocations.
 *
 * @see iotc_bsp_tls_init
 * @see iotc_bsp_tls_send_callback
 * @see iotc_bsp_tls_recv_callback
 */
typedef struct iotc_bsp_tls_init_params_s {
  /** Callback context. Required by <code>iotc_bsp_tls_send_callback()</code>
   * and <code>iotc_bsp_tls_recv_callback()</code>. */
  void* libiotc_io_callback_context;

  /** Pointer to a buffer with root CA PEM certificates. */
  uint8_t* ca_cert_pem_buf;
  /** The length, in bytes, of ca_cert_pem_buf. */
  size_t ca_cert_pem_buf_length;

  /** Pointer to the client application's memory allocation function. */
  void* (*fp_libiotc_alloc)(size_t);

  /** Pointer to the client application's array allocation function. */
  void* (*fp_libiotc_calloc)(size_t, size_t);

  /** Pointer to the client application's reallocation function. */
  void* (*fp_libiotc_realloc)(void*, size_t);

  /** Pointer to the client application's free memory function. */
  void (*fp_libiotc_free)(void*);

  /** A pointer to the host's domain name. Must be a null-terminated
   * string. */
  const char* domain_name;

} iotc_bsp_tls_init_params_t;

/**
 * @typedef iotc_bsp_tls_context_t
 * @brief TLS context representation.
 *
 * The Device SDK doesn't read or write to this context directly, but
 * provides the context to the functions defined in this TLS BSP.
 * The Device SDK calls <code>iotc_bsp_tls_init()</code> to create a
 * TLS context.
 *
 * @see iotc_bsp_tls_init
 */
typedef void iotc_bsp_tls_context_t;

/**
 * @function
 * @brief Initialize a TLS library and create a TLS context.
 *
 * The Device SDK calls this function and then deletes the init_params.
 * Any persistant data in init_params must be stored outside the scope of
 * this function.
 *
 * @see iotc_bsp_tls_context_t
 * @see iotc_bsp_tls_init_params_t
 *
 * @param [out] tls_context A pointer to a TLS context.
 * @param [in] init_params Data required to create a TLS context. Refer to the
 *     iotc_bsp_tls_init_params_t Definition for more information.
 *
 * @retval IOTC_BSP_TLS_STATE_OK The TLS context is successfully created.
 * @retval IOTC_BSP_TLS_STATE_INIT_ERROR Can't initialize TLS library.
 */
iotc_bsp_tls_state_t iotc_bsp_tls_init(iotc_bsp_tls_context_t** tls_context,
                                       iotc_bsp_tls_init_params_t* init_params);
/**
 * @function
 * @brief Free a TLS context.
 *
 * After closing a connection, the Device SDK will invoke this function
 * to free the corresponding memory resources and delete any associated data.
 *
 * @see iotc_bsp_tls_context_t
 *
 * @param [in|out] tls_context The iotc_bsp_tls_context_t TLS context
 *     representation.
 *
 * @retval IOTC_BSP_TLS_STATE_OK TLS context successfully freed.
 */
iotc_bsp_tls_state_t iotc_bsp_tls_cleanup(iotc_bsp_tls_context_t** tls_context);

/**
 * @function
 * @brief Start TLS connections.
 *
 * The Device SDK calls this function to start a TLS handshake. If the function
 * returns IOTC_BSP_TLS_STATE_WANT_WRITE or IOTC_BSP_TLS_STATE_WANT_READ, the
 * Device SDK will invoke the function again to complete the handshake.
 *
 * @see iotc_bsp_tls_context_t
 *
 * @param [in] tls_context The iotc_bsp_tls_context_t TLS context
 *     representation.

 * @retval IOTC_BSP_TLS_STATE_OK The TLS handshake successfully completed.
 * @retval IOTC_BSP_TLS_STATE_WANT_READ The TLS handshake is partially complete.
 *     The function will be invoked again to read the remaining data from the
 *     socket specified in the tls_context parameter.
 * @retval IOTC_BSP_TLS_STATE_WANT_WRITE The TLS handshake is partially
 *     complete. The function will be invoked again to write the remaining data
 *     to the socket specified in the tls_context parameter.
 * @retval IOTC_BSP_TLS_STATE_CERT_ERROR Can't validate CA certificate.
 * @retval IOTC_BSP_TLS_STATE_CONNECT_ERROR Can't complete TLS handshake.
 */
iotc_bsp_tls_state_t iotc_bsp_tls_connect(iotc_bsp_tls_context_t* tls_context);

/**
 * @function
 * @brief Read data on a socket.
 *
 * @see iotc_bsp_tls_context_t
 *
 * @param [in] tls_context The iotc_bsp_tls_context_t TLS context
 *     representation.
 * @param [in] data_ptr A pointer to a buffer to store data that is read.
 * @param [in] data_size The size, in bytes, of the buffer to which data_ptr
 *     points.
 * @param [out] bytes_read The number of bytes read.
 *
 * @retval IOTC_BSP_TLS_STATE_OK All data is successfully read and stored in
 *     the buffer to which data_ptr points.
 * @retval IOTC_BSP_TLS_STATE_WANT_READ The TLS handshake is partially complete.
 *     The function will be called again to read the remaining data from the
 *     socket specified in the tls_context parameter.
 * @retval IOTC_BSP_TLS_STATE_READ_ERROR Can't read data.
 */
iotc_bsp_tls_state_t iotc_bsp_tls_read(iotc_bsp_tls_context_t* tls_context,
                                       uint8_t* data_ptr, size_t data_size,
                                       int* bytes_read);

/**
 * @function
 * @brief Count the pending readable bytes.
 *
 * Return the the number of bytes that are available to be
 * read by the <code>iotc_bsp_tls_read</code> implementation.
 *
 * @see iotc_bsp_tls_context_t
 *
 * @param [in] tls_context The iotc_bsp_tls_context_t TLS context
 *     representation.
 *
 * @return The number of bytes available to read.
 */
int iotc_bsp_tls_pending(iotc_bsp_tls_context_t* tls_context);

/**
 * @function
 * @brief Write data to a socket.
 *
 * @see iotc_bsp_tls_context_t
 *
 * @param [in] tls_context The iotc_bsp_tls_context_t TLS context
 *     representation.
 * @param [in] data_ptr A pointer to a buffer with the data to be sent.
 * @param [in] data_size the size, in bytes, of the buffer to which data_ptr
 *     points.
 * @param [out] bytes_written The number of bytes written.
 *
 * @retval IOTC_BSP_TLS_STATE_OK All data is successfully written.
 * @retval IOTC_BSP_TLS_STATE_WANT_WRITE The TLS handshake is partially
 *     complete.The function will be called again to write the remaining
 *     data to the socket specified in the tls_context parameter.
 * @retval IOTC_BSP_TLS_STATE_WRITE_ERROR Can't write data.
 */
iotc_bsp_tls_state_t iotc_bsp_tls_write(iotc_bsp_tls_context_t* tls_context,
                                        uint8_t* data_ptr, size_t data_size,
                                        int* bytes_written);

/** Notify the client application to read data on a socket. Don't modify this
 * function. */
iotc_bsp_tls_state_t iotc_bsp_tls_recv_callback(char* buf, int sz,
                                                void* context, int* bytes_sent);

/** Notify the client application to write data to a socket. Do not modify this
 * function. */
iotc_bsp_tls_state_t iotc_bsp_tls_send_callback(char* buf, int sz,
                                                void* context, int* bytes_sent);

#endif /* __IOTC_BSP_TLS_H__ */
