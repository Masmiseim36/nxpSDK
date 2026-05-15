/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MU_IPC_SHM_CLIENT__H_
#define _MU_IPC_SHM_CLIENT__H_

#include "cc.h"
#include "mu_ipc_shm.h"

void IPC_SHM_MU_IRQHandler(void);

#define PDO_write_start()     ipc_shm_client_pdc_write_start(&pdo_comm)
#define PDO_write_end()       ipc_shm_client_pdc_write_end(&pdo_comm)
#define PDO_read_start()      ipc_shm_client_pdc_read_start(&pdo_comm)
#define PDO_read_end()        ipc_shm_client_pdc_read_end(&pdo_comm)

void obj_handler();
void MU_ipc_shm_client_init();
void process_handler();

int func_call_handler();
#endif
