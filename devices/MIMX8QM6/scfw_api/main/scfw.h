/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * Header file containing includes to system headers and porting types.
 */

#ifndef SC_SCFW_H
#define SC_SCFW_H

/* Includes */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/*!
 * This type is used to declare a handle for an IPC communication
 * channel. Its meaning is specific to the IPC implementation.
 */
typedef uint32_t sc_ipc_t;

/*!
 * This type is used to declare an ID for an IPC communication
 * channel. Its meaning is specific to the IPC implementation.
 */
typedef uint32_t sc_ipc_id_t;

#endif /* SC_SCFW_H */

