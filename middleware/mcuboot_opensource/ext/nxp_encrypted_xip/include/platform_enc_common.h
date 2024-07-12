/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PLATFORM_ENC_COMMON_H__
#define __PLATFORM_ENC_COMMON_H__

#include "fsl_common.h"
#include "flash_map.h"

/* Checks whether active slot has valid encryption metadata.  */
status_t platform_enc_check_slot(struct flash_area *fa, uint32_t *active_slot);

/* Initializes new encryption metadata with random nonce in active slot */
status_t platform_enc_prepare_slot(struct flash_area *fa, uint32_t active_slot);

/* Initializes platform on-the-fly encryption based on configuration in encryption metadata */
status_t platform_enc_init_slot(struct flash_area *fa, uint8_t *nonce);

/* Deinitializes platform on-the-fly encryption */
status_t platform_enc_deinit_slot(struct flash_area *fa);

/* Finish initialization of on-the-fly encryption (if needed) */
status_t platform_enc_finish(struct flash_area *fa);

/* Encrypts data */
status_t platform_enc_encrypt_data(uint32_t flash_addr, uint8_t *nonce, uint8_t *input, uint8_t *output, uint32_t len);

#endif
