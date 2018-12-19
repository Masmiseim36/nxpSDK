/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _SSLSRV_PORT_H_
#define _SSLSRV_PORT_H_

#include "FreeRTOS.h"

#define sslsrv_mem_alloc(x) pvPortMalloc(x)
#define sslsrv_mem_free(x) vPortFree(x)

/* Function used by mbedtls to send data */
int sslsrv_mbedtls_send(void *ctx, unsigned char const *buf, size_t len);

/* Function used by mbedtls to receive data */
int sslsrv_mbedtls_recv(void *ctx, unsigned char *buf, size_t len);

/* Initialization of server socket - create, bind and start listening */
int sslsrv_init_socket(volatile int *sock, uint32_t max_ses);

/* Wait for connection of client, return socket with activity or error */
int sslsrv_wait_for_conn(int sock);

/* Accept connection from client */
int sslsrv_accept(int sock);

/* Close connection */
int sslsrv_close(int sock);

#endif
