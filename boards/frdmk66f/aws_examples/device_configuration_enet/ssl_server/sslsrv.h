/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   Header for SSLSRV.*/

#ifndef _SSLSRV_H_
#define _SSLSRV_H_

#include "sslsrv_config.h"
#include <stdint.h>

/*
** SSL server return codes
*/
#define SSLSRV_OK 0
#define SSLSRV_ERR -1
#define SSLSRV_BIND_FAIL -2
#define SSLSRV_LISTEN_FAIL -3
#define SSLSRV_CREATE_FAIL -5
#define SSLSRV_BAD_FAMILY -6
#define SSLSRV_SOCKOPT_FAIL -7
#define SSLSRV_SES_INVALID -8
#define SSLSRV_FAIL -9

/*
** SSL server TLS parameters
*/
typedef struct sslsrv_tls_param_struct
{
    const unsigned char *certificate_buffer;
    unsigned long certificate_buffer_size;
    const unsigned char *private_key_buffer;
    unsigned long private_key_buffer_size;
} SSLSRV_TLS_PARAM_STRUCT;

/*
** SSL server parameters
*/
typedef struct sslsrv_param_struct
{
    uint32_t max_ses;                         /* maximal sessions count */
    uint32_t task_prio;                       /* server task priority */
    const SSLSRV_TLS_PARAM_STRUCT *tls_param; /* TLS parameters. */
} SSLSRV_PARAM_STRUCT;

#ifdef __cplusplus
extern "C" {
#endif

/*
** Initialize and run SSL server
** Returns server handle when successful, zero otherwise.
*/
uint32_t SSLSRV_init(SSLSRV_PARAM_STRUCT *params);

/*
** Stop and release SSL server
** Returns SSLSRV_OK when successful, SSLSRV_ERR otherwise.
*/
void SSLSRV_release(uint32_t server_h);

#ifdef __cplusplus
}
#endif

#endif /* _SSLSRV_H_ */
