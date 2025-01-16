/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ENCRYPTED_XIP_H__
#define __ENCRYPTED_XIP_H__

#include "fsl_common.h"
#include "flash_map.h"

/*
 * Identify type of encryption unit.
 *
 * These CPU macros should be declared in the project or makefile.
 */
#if !(defined(ENCRYPTED_XIP_BEE) || defined(ENCRYPTED_XIP_OTFAD) || \
      defined(ENCRYPTED_XIP_NPX) || defined(ENCRYPTED_XIP_IPED))
#error "Please define target encryption unit type"
#endif

/* Preliminary initialization needed by encryption unit  */
status_t encrypted_xip_init(void);

/* Checks whether encryption metadata in metadata sector are valid and confirmed
 * Returns number of active (referenced) slot */
status_t encrypted_xip_cfg_check(struct flash_area *fa_meta, bool *is_valid, uint32_t *active_slot);

/* Initializes new configuration block of encryption unit and write it into metadata sector */
status_t encrypted_xip_cfg_write(struct flash_area *fa_meta);

/* Initializes encryption unit from configuration block in metadata sector */
status_t encrypted_xip_cfg_initEncryption(struct flash_area *fa_meta);

/* Confirm configuration block of encryption unit by completion of metadata sector */
status_t encrypted_xip_cfg_confirm(struct flash_area *fa_meta, uint32_t active_slot);

/* Get nonce from configuration block in metadata sector */
status_t encrypted_xip_cfg_getNonce(struct flash_area *fa_meta, uint8_t *nonce);

/* Finish initialization of encryption unit */
status_t encrypted_xip_finish(void);

/* Encrypts data */
status_t encrypted_xip_encrypt_data(uint32_t flash_addr, uint8_t *nonce, uint8_t *input, uint8_t *output, uint32_t len);

/* Helper function for writing to flash within encrypted region */
status_t encrypted_xip_flash_write(const struct flash_area *area, uint32_t off, const void *src, uint32_t len);

#endif
