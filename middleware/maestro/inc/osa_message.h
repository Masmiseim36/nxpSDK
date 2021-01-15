/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_MESSAGE_H
#define OSA_MESSAGE_H

/*!
 * @file    osa_message.h
 * @brief   Contains message queue function prototypes for libosa.
 */

/*!
 * @ingroup libosa
 * @brief   Open a message queue
 * @details Open a connection to message queue identified by 'name'
 * @param   queue A queue object will be initialized and returned to the user
 *          in this parameter
 * @param   name The name of the message queue.  This must start with a
 *          forward slash (ex. /my_message_queue)
 * @param   max_size The maximum size of a message sent and received on the
 *          queue
 * @param   blocking Boolean representing whether or not the queue will block
 *          when sending and receiving messages
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid parameter.  Queue may exist
 *                       already, queue name may be invalid or too long, or
 *                       max_size exceeds limits.
 * @retval  ERRCODE_PERMISSION_DENIED    Permission denied to open queue
 *                      (Linux only)
 * @retval  ERRCODE_SYSTEM_LIMIT   Exceeded system limits on number of open
 *                      queues
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_mq_open(OsaMq *queue, const char *name, uint32_t max_size,
                bool blocking);

/*!
 * @ingroup libosa
 * @brief   Add a message to the queue
 * @details Add a message of a given size onto the given queue
 * @param   queue A valid queue object
 * @param   msg A pointer to the message buffer
 * @param   size The size of the message
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_OUT_OF_MEMORY   Out of memory
 * @retval  ERRCODE_TRY_AGAIN   The queue is full and non-blocking was specified
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid parameter.  Queue object may be
 *                                     invalid or message size exceeds limits.
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_mq_send(OsaMq *queue, const char *msg, size_t size);

/*!
 * @ingroup libosa
 * @brief   Receive a message from a queue
 * @details Receive a message up to the given size from a queue
 * @param   queue A valid queue object
 * @param   msg A pointer to a buffer to store the received message.  The
 *          buffer must be at least 'size' bytes large.
 * @param   size The maximum size of message to be received
 * @param   msec_timeout The maximum number of milliseconds to block for a
 *          message, if blocking is enabled.  Passing 0 will block forever.
 * @param   receive_size An output parameter where the size of the received
 *          message will be stored.  Will be 0 on failure.  Will not exceed
 *          'size'.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_TRY_AGAIN   The queue is empty and non-blocking was
 *                      specified
 * @retval  ERRCODE_TIMED_OUT No message received in the timeout period
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid parameter.  Queue object may be
 *                                     invalid or message size exceeds limits.
 * @retval  ERRCODE_PERMISSION_DENIED  XXX: mqx shouldn't return this?
 * @retval  ERRCODE_GENERAL_ERROR      Unexpected system error
*/
int osa_mq_receive(OsaMq *queue, char *msg, size_t size,
                   uint32_t msec_timeout, size_t *receive_size);

/*!
 * @ingroup libosa
 * @brief   Receive a message from a queue
 * @details Receive a message up to the given size from a queue, never
 *          blocking for any amount of time if there is no message to be
 *          received.
 * @param   queue A valid queue object
 * @param   msg A pointer to a buffer to store the received message.  The
 *          buffer must be at least 'size' bytes large.
 * @param   size The maximum size of message to be received
 * @param   receive_size An output parameter where the size of the received
 *          message will be stored.  Will be 0 on failure.  Will not exceed
 *          'size'.
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_TRY_AGAIN   The queue is empty and non-blocking was
 *                      specified
 * @retval  ERRCODE_TIMED_OUT No message received in the timeout period
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid parameter.  Queue object may be
 *                                     invalid or message size exceeds limits.
 * @retval  ERRCODE_PERMISSION_DENIED  XXX: mqx shouldn't return this?
 * @retval  ERRCODE_GENERAL_ERROR      Unexpected system error
*/
int osa_mq_poll(OsaMq *queue, char *msg, size_t size, size_t *receive_size);

/*!
 * @ingroup libosa
 * @brief   Close a message queue
 * @details Closes an open connection to a message queue but does not destroy
 *          the queue.
 * @param   queue A valid queue object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid parameter.  Queue object may be
 *                      invalid.
 * @retval  ERRCODE_PERMISSION_DENIED    XXX: mqx shouldn't return this?
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_mq_close(OsaMq *queue);

/* Destroys a message queue and releases all memory allocated by it. */

/*!
 * @ingroup libosa
 * @brief   Destroy a message queue
 * @details Destroys a message queue and releases all memory allocated by it.
 * @param   name Name of a valid queue object
 *
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_INVALID_ARGUMENT   Invalid parameter.  Queue object may be
 *                      invalid.
 * @retval  ERRCODE_GENERAL_ERROR   Unexpected system error
*/
int osa_mq_destroy(const char *name);

#endif

/*! @} */

