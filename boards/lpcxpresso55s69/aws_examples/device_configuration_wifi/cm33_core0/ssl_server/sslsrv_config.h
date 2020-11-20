/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*   Default configuration for SSLSRV.*/

#ifndef _SSLSRV_CONFIG_H_
#define _SSLSRV_CONFIG_H_

/* Configuration parameters.*/

/* Listener task stack size */
#ifndef SSLSRV_CFG_SERVER_STACK_SIZE
#define SSLSRV_CFG_SERVER_STACK_SIZE (3000 / sizeof(uint32_t))
#endif

/* Session task stack size.*/
#ifndef SSLSRV_CFG_SESSION_STACK_SIZE
#define SSLSRV_CFG_SESSION_STACK_SIZE (2500 / sizeof(uint32_t))
#endif

/* Task priority - must be lower than priority of network stack task */
#ifndef SSLSRV_CFG_DEFAULT_PRIORITY
#define SSLSRV_CFG_DEFAULT_PRIORITY 2
#endif

/* Default port */
#ifndef SSLSRV_CFG_DEFAULT_PORT
#define SSLSRV_CFG_DEFAULT_PORT (443)
#endif

/* Default sessions count */
#ifndef SSLSRV_CFG_DEFAULT_SES_CNT
#define SSLSRV_CFG_DEFAULT_SES_CNT (1)
#endif

/* Session buffer size */
#ifndef SSLSRV_CFG_SES_BUFFER_SIZE
#define SSLSRV_CFG_SES_BUFFER_SIZE (2048) //(1360)
#endif

/* Session timeout in milliseconds */
#ifndef SSLSRV_CFG_SES_TIMEOUT
#define SSLSRV_CFG_SES_TIMEOUT (120000)
#endif

#endif /* _SSLSRV_CONFIG_H_ */
