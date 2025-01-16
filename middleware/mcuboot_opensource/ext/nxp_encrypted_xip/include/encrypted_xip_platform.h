/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ENCRYPTED_XIP_PLATFORM_H__
#define __ENCRYPTED_XIP_PLATFORM_H__

#include "fsl_common.h"
#include "flash_map.h"

status_t platform_enc_init(void);

size_t platform_enc_cfg_getSize(void);

status_t platform_enc_cfg_write(struct flash_area *fa_meta);

status_t platform_enc_cfg_initEncryption(struct flash_area *fa_meta);

bool platform_enc_cfg_isPresent(uint32_t addr);

status_t platform_enc_cfg_getNonce(struct flash_area *fa_meta, uint8_t *nonce);

status_t platform_enc_finish(void);

status_t platform_enc_encrypt_data(uint32_t flash_addr, uint8_t *nonce, uint8_t *input, uint8_t *output, uint32_t len);

status_t platform_enc_flash_write(const struct flash_area *area, uint32_t off, const void *src, uint32_t len);





#endif
