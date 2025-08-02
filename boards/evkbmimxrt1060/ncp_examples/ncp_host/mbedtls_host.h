/** @file mbedtls_host.h
 *
 *  @brief This file provides NCP host mbedtls interfaces.
 *
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MBEDTLS_HOST_H__
#define __MBEDTLS_HOST_H__
#include "stdint.h"

int ncp_process_encrypt_event(uint8_t *res);
int ncp_process_encrypt_stop_event(uint8_t *res);
int ncp_process_encrypt_response(uint8_t *res);
int ncp_encrypt_command(int argc, char **argv);
int ncp_dbg_encrypt_stop_command(int argc, char **argv);
int ncp_trigger_encrypted_communication(void);

#endif /* __MBEDTLS_HOST_H__ */
