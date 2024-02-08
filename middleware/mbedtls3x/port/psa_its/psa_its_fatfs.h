/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PSA_ITS_FATFS_H
#define PSA_ITS_FATFS_H

/** \file psa_its_fatfs.h
 *
 */

#include "psa/crypto.h"
#include "mcux_psa_s4xx_common_init.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Create the top level directory for NVM storage
 *
 * \retval  0 on success. 
 *          -1 on failure
 */
int sd_its_fs_initialize(void);

#ifdef __cplusplus
}
#endif
#endif /* PSA_ITS_FATFS_H */
