/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SSLSRV_PRV_H_
#define _SSLSRV_PRV_H_

#include "sslsrv.h"
#include "sslsrv_tls.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define SSLSRV_VALID (1)
#define SSLSRV_INVALID (0)
#define SSLSRV_SES_BUF_SIZE_PRV (SSLSRV_CFG_SES_BUFFER_SIZE)

/*
 * SSL session buffer
 */
typedef struct sslsrv_buffer
{
    uint32_t offset; /* Write offset */
    char *data;      /* Buffer data */
} SSLSRV_BUFF_STRUCT;

/*
 * Session process function prototype
 */
typedef void (*SSLSRV_SES_FUNC)(void *server, void *session);

/*
 * SSL session structure
 */
typedef struct sslsrv_session_struct
{
    SSLSRV_SES_FUNC process_func; /* Session process function */
    volatile uint32_t valid;      /* Any value different than SSLSRV_VALID means session is invalid */
    volatile int sock;            /* Session socket */
    volatile uint32_t
        time; /* Session time in ticks. Updated when there is some activity in session. Used for timeout detection. */
    uint32_t timeout;          /* Session timeout in ticks. timeout_time = time + timeout */
    SSLSRV_BUFF_STRUCT buffer; /* Session internal read/write buffer */
    sslsrv_tls_sock_t tls_sock;
    bool authenticated; /* Client connected in this session is/isn't authenticated (can/can't do some requests) */
} SSLSRV_SESSION_STRUCT;

/*
** SLL server main structure.
*/
typedef struct sslsrv_struct
{
    SSLSRV_PARAM_STRUCT params;               /* server parameters */
    volatile int sock;                        /* listening socket*/
    SSLSRV_SESSION_STRUCT *volatile *session; /* array of pointers to sessions */
    volatile uint32_t valid;                  /* Any value different than SSLSRV_VALID means session is invalid */
    volatile TaskHandle_t server_tid;         /* Server task ID */
    SemaphoreHandle_t ses_cnt;                /* Session counter */
    sslsrv_tls_ctx_t tls_ctx;                 /* TLS context.*/
} SSLSRV_STRUCT;

/*
** Parameter for session task
*/
typedef struct sslsrv_ses_task_param
{
    SSLSRV_STRUCT *server; /* Pointer to server structure */
    SSLSRV_SESSION_STRUCT *volatile *session_p;
} SSLSRV_SES_TASK_PARAM;

#ifdef __cplusplus
extern "C" {
#endif

void sslsrv_server_task(void *arg);
void sslsrv_process(void *server_ptr, void *session_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _SSLSRV_PRV_H_ */
