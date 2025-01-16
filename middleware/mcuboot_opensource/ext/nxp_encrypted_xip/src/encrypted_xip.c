/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sblconfig.h"
#if defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE)
#include <ctype.h>

#include "encrypted_xip.h"
#include "encrypted_xip_platform.h"
#include "fsl_debug_console.h"

#include "mflash_drv.h"
   
#include "flash_partitioning.h"

#include "mbedtls/md5.h"

#include "mflash_drv.h"
#include "sysflash/sysflash.h"
#include "flash_map.h"
#include "flash_partitioning.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * @brief Helper to assert function return status.
 *
 * @details Print finshed message and trap forever upon failure.
 */
#define ASSERT_APP(expected, actual, ...) \
    do                                    \
    {                                     \
        if (expected != actual)           \
        {                                 \
            PRINTF("Assertion failed: ");   \
            PRINTF(__VA_ARGS__);          \
            while (1)                     \
                ;                         \
        }                                 \
    } while (0);

#define ENC_MAGIC_SZ 16

/*******************************************************************************
 * Types
 ******************************************************************************/

/** Union holds magic number for encryption metadata block identification.
 */
union enc_magic_t {
	//ToDo MCUBOOT_BOOT_MAX_ALIGN ?
	uint8_t val[ENC_MAGIC_SZ];
};

/** Structure holds metadata used for confirmation of configuration blocks
 */
typedef struct {
	uint32_t active_slot;           // referenced slot: 0 - primary, 1 - secondary
	uint8_t pad_0[12];              // Padding zeroes
	uint8_t hash[16];               // Hash of configuration structure
	uint8_t pad_1[16];              // Padding zeroes
	uint8_t magic[ENC_MAGIC_SZ];    // Magic number
} enc_metadata_t;

_Static_assert((sizeof(enc_metadata_t) % 16) == 0,
		"Unaligned size of enc_metadata_t struct");


/*
 * Assume that metadata and configuration structs can be written separately 
 * into common flash sector.
 */
_Static_assert(sizeof(enc_metadata_t) <= MFLASH_PAGE_SIZE,
		"Size of metadata exceeds flash page size");

_Static_assert(sizeof(enc_metadata_t) <= MFLASH_SECTOR_SIZE/2,
		"Size of metadata exceeds half flash sector size");
/*******************************************************************************
 * Variables
 ******************************************************************************/
const union enc_magic_t enc_magic = { .val = { 0xAA, 0xBB, 0xCC, 0xDD, 0x60,
		0x4D, 0xBA, 0x70, 0x34, 0x79, 0x2c, 0x0f, 0x2c, 0xb6, 0x0f, 0x35 } };

#define ENC_MAGIC  (enc_magic.val)
/*******************************************************************************
 * Static
 ******************************************************************************/
static int magic_check(const uint8_t *magic) {
    if (memcmp(magic, ENC_MAGIC, ENC_MAGIC_SZ) == 0) {
        return 1;
    }
    return 0;
}

static void hexdump(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    const int CNT             = 16;

    for (size_t i = 0; i < size; i++)
    {
        int n = i % CNT;
        if (n == 0)
            PRINTF("%08x  ", i);
        PRINTF("%02X ", src8[i]);
        if ((i && n == CNT - 1) || i + 1 == size)
        {
            int rem = CNT - 1 - n;
            for (int j = 0; j < rem; j++)
                PRINTF("   ");
            PRINTF("|");
            for (int j = n; j >= 0; j--)
                PUTCHAR(isprint(src8[i - j]) ? src8[i - j] : '.');
            PRINTF("|\n");
        }
    }
    PUTCHAR('\n');
}
/*******************************************************************************
 * Externs
 ******************************************************************************/
status_t encrypted_xip_init(void)
{
    return platform_enc_init();
}

status_t encrypted_xip_cfg_check(struct flash_area *fa_meta, bool *is_valid, uint32_t *active_slot)
{
    enc_metadata_t metadata;
    uint32_t cfg_block[1024 / sizeof(uint32_t)];
    /* metadata are located at the end of sector */
    const uint32_t off_meta = fa_meta->fa_size - sizeof(enc_metadata_t);
    /* configuration block is located at the beginning of sector */
    const uint32_t off_cfg = 0;
    
    *is_valid = false;
    
    uint32_t cfg_sz = platform_enc_cfg_getSize();
    ASSERT_APP(0 < 1, cfg_sz <= 1024, "Configuration size exceeds size of the buffer\n");    
   
    if (flash_area_read(fa_meta, off_meta, &metadata, sizeof(enc_metadata_t)) != 0){
        return kStatus_Fail;
    }
    if (flash_area_read(fa_meta, off_cfg, cfg_block, cfg_sz) != 0){
        return kStatus_Fail;
    }
    if (magic_check(metadata.magic)) {
        /* Check hash */
        mbedtls_md5_context md_ctx;
        uint8_t md[16];

        mbedtls_md5_init(&md_ctx);
        mbedtls_md5_starts_ret(&md_ctx);
        mbedtls_md5_update(&md_ctx, (unsigned char*) cfg_block, cfg_sz);
        mbedtls_md5_finish(&md_ctx, md);

        if (memcmp(metadata.hash, md, 16) == 0) {
            if (metadata.active_slot == 0 || metadata.active_slot == 1) {
              *is_valid = true;
              if (active_slot != NULL){
                    *active_slot = metadata.active_slot;
              }
            }
        }
    }

    return kStatus_Success;   
}

status_t encrypted_xip_cfg_write(struct flash_area *fa_meta)
{ 
    return platform_enc_cfg_write(fa_meta);
}

status_t encrypted_xip_cfg_initEncryption(struct flash_area *fa_meta)
{
    return platform_enc_cfg_initEncryption(fa_meta);
}

/* In case of PRINCE based encryption units there is a risk when accessing to
 * unwritten pages which leads to crash of PRINCE module.
 * This operation ensures the integrity of IPED configuration of the image in
 * execution area.
 */
status_t encrypted_xip_cfg_confirm(struct flash_area *fa_meta, uint32_t active_slot)
{
    status_t status = kStatus_Fail;
    uint32_t meta_off = fa_meta->fa_size - sizeof(enc_metadata_t);
    const uint32_t cfg_addr = fa_meta->fa_off + BOOT_FLASH_BASE;
    enc_metadata_t metadata;

    /* Sanity check the configuration is valid */
    bool cfg_isPresent = platform_enc_cfg_isPresent(cfg_addr);
    if (cfg_isPresent == false) {
        PRINTF("No configuration block found!\n");
        goto error;
    }
    
    /* Calculate hash */
    mbedtls_md5_context md_ctx;
    uint8_t md[16];
    uint32_t len;

    len = platform_enc_cfg_getSize();
    ASSERT_APP(0 , len % 16, "Unaligned size of IPED struct len=%d\n", len);

    mbedtls_md5_init(&md_ctx);
    mbedtls_md5_starts_ret(&md_ctx);
    mbedtls_md5_update(&md_ctx, (unsigned char*) cfg_addr, len);
    mbedtls_md5_finish(&md_ctx, md);

    memset(&metadata, 0, sizeof(enc_metadata_t));
    memcpy(metadata.hash, md, 16);
    metadata.active_slot = active_slot;
    memcpy(metadata.magic, ENC_MAGIC, ENC_MAGIC_SZ);

    /*Write metadata at the end of sector - confirm integrity of configuration*/
    if (flash_area_write(fa_meta, meta_off, &metadata, sizeof(enc_metadata_t)) != 0) {
        PRINTF("Failed to write encryption metadata\n");
        goto error;
    }

    return kStatus_Success;
    error: 
    return kStatus_Fail;
}

status_t encrypted_xip_cfg_getNonce(struct flash_area *fa_meta, uint8_t *nonce)
{  
    return platform_enc_cfg_getNonce(fa_meta, nonce);
}

status_t encrypted_xip_finish(void)
{
    return platform_enc_finish();
}

status_t encrypted_xip_encrypt_data(uint32_t flash_addr, uint8_t *nonce, 
                                   uint8_t *input, uint8_t *output, uint32_t len)
{
    return platform_enc_encrypt_data(flash_addr, nonce, input, output, len);
}

status_t encrypted_xip_flash_write(const struct flash_area *area, uint32_t off, const void *src, uint32_t len)
{
    return platform_enc_flash_write(area, off, src, len);
}

/*******************************************************************************
 * Test
 ******************************************************************************/

#if 0
#define FLASH_PAGE_SIZE         xxx

void hexdump(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    const int CNT             = 16;

    for (size_t i = 0; i < size; i++)
    {
        int n = i % CNT;
        if (n == 0)
            PRINTF("%08x  ", i);
        PRINTF("%02X ", src8[i]);
        if ((i && n == CNT - 1) || i + 1 == size)
        {
            int rem = CNT - 1 - n;
            for (int j = 0; j < rem; j++)
                PRINTF("   ");
            PRINTF("|");
            for (int j = n; j >= 0; j--)
                PUTCHAR(isprint(src8[i - j]) ? src8[i - j] : '.');
            PRINTF("|\n");
        }
    }
    PUTCHAR('\n');
}



void dump_image(void)
{
  static uint32_t buffer[MFLASH_PAGE_SIZE / sizeof(uint32_t)];
  const struct flash_area *fap_src = boot_enc_flash_map;
  const struct flash_area *fap_dst = boot_dump_flash_map;
  uint32_t bytes_copied = 0;
  uint32_t cur_off = 0;
  uint32_t dump_size = 524288; // 0,5MB
  
  while(bytes_copied < dump_size)
  {
    memcpy(buffer, (void*) (BOOT_FLASH_ENC_PRT + cur_off), MFLASH_PAGE_SIZE);
    if((cur_off % MFLASH_SECTOR_SIZE) == 0){
      if(flash_area_erase(fap_dst, cur_off, MFLASH_SECTOR_SIZE) != 0){
        PRINTF("Flash erase failed");
        goto error;
      }
      PUTCHAR('.');
    }
    if(flash_area_write(fap_dst, cur_off, buffer, MFLASH_PAGE_SIZE) != 0){
      PRINTF("Flash write failed");
      goto error;
    }
    cur_off += MFLASH_PAGE_SIZE;
    bytes_copied += MFLASH_PAGE_SIZE;
  }
  return;
error:
  PRINTF("Dump failed");
}
#endif

#endif /* CONFIG_ENCRYPT_XIP_EXT_ENABLE */
