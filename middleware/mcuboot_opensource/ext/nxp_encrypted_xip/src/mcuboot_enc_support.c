/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "sblconfig.h"
#ifdef CONFIG_MCUBOOT_ENCRYPTED_XIP_SUPPORT

#include <stdint.h>

#include "fsl_debug_console.h"
#include "mcuboot_enc_support.h"
#include "platform_enc_common.h"
#include "mflash_drv.h"

#include "sysflash/sysflash.h"
#include "flash_map.h"
#include "flash_partitioning.h"
#include "bootutil/bootutil_log.h"
#include "bootutil/image.h"

#include "bootutil/enc_key.h"
#include "mbedtls/rsa.h"
#include "bootutil/crypto/sha.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (BOOT_FLASH_ACT_APP % MFLASH_SECTOR_SIZE != 0) || \
    (BOOT_FLASH_CAND_APP % MFLASH_SECTOR_SIZE != 0)|| \
    (BOOT_FLASH_ENC_APP % MFLASH_SECTOR_SIZE != 0)
#error "Incorrect aligment of image slots"
#endif

#define PRIMARY_SLOT_OFFSET     (BOOT_FLASH_ACT_APP - BOOT_FLASH_BASE)
#define SECONDARY_SLOT_OFFSET   (BOOT_FLASH_CAND_APP - BOOT_FLASH_BASE)
#define ENCRYPTED_SLOT_OFFSET   (BOOT_FLASH_ENC_APP - BOOT_FLASH_BASE)

#define TLV_KEY_LEN             256
#define TLV_SHA256_LEN          32

#define ENC_MAGIC_SZ            16                 

/*******************************************************************************
 * Types
 ******************************************************************************/

/** Structure holds object needed by encryption process.
 */
typedef struct {
    struct image_header hdr;                    //MCUBoot image header
    struct image_tlv_info tlv_info;             //MCUBoot TLV info
    struct flash_area *fap_src;                 //Flash area of reference slot
    struct flash_area *fap_dst;                 //Flash area of execution slot
    bootutil_aes_ctr_context ctx;               //MbedTLS object for AES-CTR
    uint8_t slot;                               //Reference slot number
}enc_data_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Static
 ******************************************************************************/

static void printf_enc_data(enc_data_t *data)
{
  BOOT_LOG_INF("\nSource image \nflash device id 0x%X \nflash id 0x%X \noffset 0x%X"
               "\nsize %d bytes", 
                data->fap_src->fa_device_id,
                data->fap_src->fa_id, 
                data->fap_src->fa_off, 
                data->fap_src->fa_size);
  BOOT_LOG_INF("header size 0x%X", data->hdr.ih_hdr_size);
  BOOT_LOG_INF("img size %d bytes", data->hdr.ih_img_size);
  BOOT_LOG_INF("protected tlv size %d bytes", data->hdr.ih_protect_tlv_size);
  BOOT_LOG_INF("tlv tot size %d bytes", data->tlv_info.it_tlv_tot);
  
  BOOT_LOG_INF("\nDestination encrypted image \nflash device id 0x%X \nflash id 0x%X"
               "\noffset 0x%X \nsize %d bytes", 
                data->fap_src->fa_device_id,
                data->fap_dst->fa_id, 
                data->fap_dst->fa_off, 
                data->fap_dst->fa_size);
}

/**
 * Decrypts data in buffer.
 *
 * @param  aes_ctx      The AES context-type structure context.
 * @param  off_nonce    The offset indicating nonce counter in AES-CTR
 * @param  sz           Size of buffer to be decrypted
 * @param  blk_off      The bulk offset
 * @param  buf          Buffer containing encrypted data
 *
 * @return              kStatus_Success on success; otherwise on failure.
 */
static int decrypt(bootutil_aes_ctr_context *aes_ctx, uint32_t off_nonce, 
                        uint32_t sz, uint32_t blk_off, uint8_t *buf)
{
    uint8_t nonce[16];

    memset(nonce, 0, 12);
    off_nonce >>= 4;
    nonce[12] = (uint8_t)(off_nonce >> 24);
    nonce[13] = (uint8_t)(off_nonce >> 16);
    nonce[14] = (uint8_t)(off_nonce >> 8);
    nonce[15] = (uint8_t)off_nonce;

    return bootutil_aes_ctr_encrypt(aes_ctx, nonce, buf, sz, blk_off, buf);
}

/**
 * Decrypt and load AES-CTR key in TLV section of the referenced image.
 *
 * @param  data      Pointer to encryption context object
 *
 * @return           kStatus_Success on success; otherwise on failure.
 */
static status_t find_and_load_aes_key(enc_data_t *data)
{
  struct flash_area *fap;
  struct image_header *hdr;
  struct image_tlv_iter it;
  uint8_t slot;
  uint8_t tlv_enc_key[TLV_KEY_LEN];
  uint32_t tlv_offset;
  uint16_t tlv_len;
  uint8_t dec_aes_key[BOOT_ENC_KEY_ALIGN_SIZE];
  
  fap = data->fap_src;
  hdr = &data->hdr;
  
  /* Locate TLV section with AES-CTR key */
  if(bootutil_tlv_iter_begin(&it, hdr, fap, IMAGE_TLV_ENC_RSA2048, false))
    goto error;
  if(bootutil_tlv_iter_next(&it, &tlv_offset, &tlv_len, NULL))
    goto error;
  if (tlv_len != BOOT_ENC_TLV_SIZE)
    goto error;
  
  if(flash_area_read(fap, tlv_offset, tlv_enc_key, BOOT_ENC_TLV_SIZE) != 0)
  {
    BOOT_LOG_INF("Flash read failed");
    goto error;
  }
  
  /* Decrypt AES-CTR key and load it */
  if(boot_enc_decrypt(tlv_enc_key, dec_aes_key) != 0)
  {
    BOOT_LOG_INF("Failed to decrycpt AES key in TLV section");
    goto error;
  }
  
  if(bootutil_aes_ctr_set_key(&data->ctx, dec_aes_key) != 0)
    goto error;
  
  BOOT_LOG_INF("AES-CTR key loaded");
  return kStatus_Success;
  error:
  BOOT_LOG_INF("Loading AES-CTR key failed");
  return kStatus_Fail;
}


#define TMPBUF_SZ       256

/**
 * Function verifies encrypted image in execution slot by comparing hash of refence 
 * image and calculated hash of the verified image. This operation also verifies
 * that on-the-fly decryption setup is functional.
 *
 * @param  data      Pointer to encryption context object.
 *
 * @return           kStatus_Success on success; otherwise on failure.
 */
static status_t verify_image(enc_data_t *data)
{
  struct flash_area *fap_src;
  struct flash_area *fap_dst;
  struct image_header *hdr;
  struct image_tlv_iter it;
  bootutil_sha_context sha256_ctx;
  uint32_t tlv_offset;
  uint32_t size;
  uint16_t tlv_len;
  uint32_t hash_src[TLV_SHA256_LEN / sizeof(uint32_t)];
  uint32_t hash_dst[TLV_SHA256_LEN / sizeof(uint32_t)];
  uint32_t tmp_buf[TMPBUF_SZ / sizeof(uint32_t)];
  uint32_t off;
  uint32_t blk_sz;
  int rc;
  
  fap_src = data->fap_src;
  fap_dst = data->fap_dst;
  hdr = &data->hdr;
  memset(hash_dst, 0, TLV_SHA256_LEN);
  
  /* Locate and load TLV SHA256 hash section of reference image */
  if(bootutil_tlv_iter_begin(&it, hdr, fap_src, IMAGE_TLV_SHA256, false))
    goto error;
  if(bootutil_tlv_iter_next(&it, &tlv_offset, &tlv_len, NULL))
    goto error;
  if (tlv_len != TLV_SHA256_LEN)
    goto error;
  if(flash_area_read(fap_src, tlv_offset, hash_src, TLV_SHA256_LEN) != 0)
  {
    BOOT_LOG_INF("Flash read failed");
    goto error;
  }
  
  /* Compute hash of re-encrypted image in execution slot so the on-the-fly */
  /* decryption is also verified */
  bootutil_sha_init(&sha256_ctx);
  /* Hash is computed over image header, payload and protected TLV (if any) */
  size = hdr->ih_hdr_size + hdr->ih_img_size + hdr->ih_protect_tlv_size;
  for (off = 0; off < size; off += blk_sz) {
        blk_sz = size - off;
        if (blk_sz > TMPBUF_SZ) {
            blk_sz = TMPBUF_SZ;
        }
        if(flash_area_read(fap_dst, off, tmp_buf, blk_sz) != 0)
        {
          BOOT_LOG_INF("Flash read failed");
          goto error;
        }
        bootutil_sha_update(&sha256_ctx, tmp_buf, blk_sz);
    }
  bootutil_sha_finish(&sha256_ctx, (uint8_t *)hash_dst);
  
  /* Compare hashes of reference and re-encrypted image */
  if(memcmp(hash_dst, hash_src, TLV_SHA256_LEN) != 0)
    goto error;
  
  BOOT_LOG_INF("Image verification successful");
  bootutil_sha_drop(&sha256_ctx);
  return kStatus_Success;
  error:
  bootutil_sha_drop(&sha256_ctx);
  BOOT_LOG_ERR("Image verification failed");
  return kStatus_Fail;
}

#define CHUNK_SIZE      1024

/**
 * Function takes referenced image by MCUBoot from staged area and re-encrypt it
 * to device specific area with platform specific encryption.
 *
 * @param  data      Pointer to encryption context object.
 * @param  nonce     Pointer to 16B nonce needed by encryption process
 *
 * @return           kStatus_Success on success; otherwise on failure.
 */
static status_t load_image(enc_data_t *data, uint8_t *nonce)
{
  static uint32_t buffer[CHUNK_SIZE / sizeof(uint32_t)];
  const struct flash_area *fap_src = data->fap_src;
  const struct flash_area *fap_dst = data->fap_dst;
  const uint32_t hdr_size = data->hdr.ih_hdr_size;
  const uint32_t tlv_off = hdr_size + data->hdr.ih_img_size;
  const uint32_t img_size = tlv_off + data->hdr.ih_protect_tlv_size 
                            + data->tlv_info.it_tlv_tot;
  uint32_t bytes_copied;
  uint32_t chunk_sz;
  uint32_t blk_sz;
  uint32_t blk_off;
  uint32_t cur_off;
  uint32_t flash_addr;
  int rc;
   
  BOOT_LOG_INF("Re-encrypting staged image to execution slot...");
  
  /* Processing image header */
  bytes_copied = 0;
  cur_off = 0;
  flash_addr = BOOT_FLASH_ENC_APP;
  while(bytes_copied < hdr_size)
  {
    
    if(flash_area_read(fap_src, cur_off, buffer, CHUNK_SIZE) != 0){
      BOOT_LOG_INF("Flash read failed");
      goto error;
    }
    if((cur_off % MFLASH_SECTOR_SIZE) == 0){
      if(flash_area_erase(fap_dst, cur_off, MFLASH_SECTOR_SIZE) != 0){
        BOOT_LOG_INF("Flash erase failed");
        goto error;
      }
    }
    if(platform_enc_encrypt_data(flash_addr + cur_off, nonce, (uint8_t *)buffer, (uint8_t *)buffer, CHUNK_SIZE) != 0){
      BOOT_LOG_INF("Encryption failed");
      goto error;
    }
    if(flash_area_write(fap_dst, cur_off, buffer, CHUNK_SIZE) != 0){
      BOOT_LOG_INF("Flash write failed");
      goto error;
    }
    cur_off += CHUNK_SIZE;
    bytes_copied += CHUNK_SIZE;
  }
  if(bytes_copied != hdr_size)
    goto error;
  
  /* Processing payload (and part of TLV) */
  bytes_copied = hdr_size;
  while(bytes_copied < tlv_off){
    if (img_size - bytes_copied > CHUNK_SIZE) {
      chunk_sz = CHUNK_SIZE;
    } else {
      chunk_sz = img_size - bytes_copied;
    }
    cur_off = bytes_copied;
    
    if(flash_area_read(fap_src, cur_off, buffer, chunk_sz) != 0){
      BOOT_LOG_INF("Flash read failed");
      goto error;
    }
    
    /* AES-CTR expects 16B alignement */
    blk_sz = chunk_sz;
    if (bytes_copied + chunk_sz > tlv_off) {
      /* Going over TLV section */
      /* Part of the chunk is encrypted payload */
      blk_off = (bytes_copied - hdr_size) & 0xF;
      blk_sz = tlv_off - bytes_copied;
      if(blk_sz > 0){
        rc = decrypt(&data->ctx, bytes_copied - hdr_size, blk_sz, blk_off, 
                     (uint8_t *)buffer);
      }
    } else {
        /* Within ecnrypted section */
        blk_off = (bytes_copied - hdr_size) & 0xF;
        rc = decrypt(&data->ctx, bytes_copied - hdr_size, blk_sz, blk_off, 
                     (uint8_t *)buffer);
    }
    if(rc != 0){
      BOOT_LOG_ERR("Block decryption failed");
      goto error;
    }
    
    if((cur_off % MFLASH_SECTOR_SIZE) == 0){
      if(flash_area_erase(fap_dst, cur_off, MFLASH_SECTOR_SIZE) != 0){
        BOOT_LOG_INF("Flash erase failed");
        goto error;
      }
      PUTCHAR('.');
    }
    if(platform_enc_encrypt_data(flash_addr + cur_off, nonce, (uint8_t *) buffer, (uint8_t *) buffer, chunk_sz) != 0){
      BOOT_LOG_INF("Encryption failed");
      goto error;
    }
    if(flash_area_write(fap_dst, cur_off, buffer, chunk_sz) != 0){
      BOOT_LOG_ERR("Flash write failed");
      goto error;
    }
    
    bytes_copied += chunk_sz;
    //TODO check 16B alingment
  }
  
  /* Processing remaining TLV if any */ 
  cur_off = bytes_copied;
  while(bytes_copied < img_size)
  {
    if (img_size - bytes_copied > CHUNK_SIZE) {
      chunk_sz = CHUNK_SIZE;
    } else {
      chunk_sz = img_size - bytes_copied;
    }
    cur_off = bytes_copied;
    
    if(flash_area_read(fap_src, cur_off, buffer, chunk_sz) != 0){
      BOOT_LOG_INF("Flash read failed");
      goto error;
    }
    if((cur_off % MFLASH_SECTOR_SIZE) == 0){
      if(flash_area_erase(fap_dst, cur_off, MFLASH_SECTOR_SIZE) != 0){
        BOOT_LOG_INF("Flash erase failed");
        goto error;
      }
    }
    if(platform_enc_encrypt_data(flash_addr + cur_off, nonce, (uint8_t *) buffer, (uint8_t *) buffer, chunk_sz) != 0){
      BOOT_LOG_INF("Encryption failed");
      goto error;
    }
    if(flash_area_write(fap_dst, cur_off, buffer, chunk_sz) != 0){
      BOOT_LOG_INF("Flash write failed");
      goto error;
    }
    bytes_copied += chunk_sz;
  }
  if(bytes_copied != img_size)
    goto error;
  
  //BOOT_LOG_INF("\nbytes_copied %d", bytes_copied);
  //BOOT_LOG_INF("chunk_sz %d", chunk_sz);
  //BOOT_LOG_INF("blk_sz %d", blk_sz);
  //BOOT_LOG_INF("blk_off %d", blk_off);
  //BOOT_LOG_INF("cur_off %d", cur_off);
  
  BOOT_LOG_INF("\nLoading image successful");
  return kStatus_Success;
  error:
  BOOT_LOG_ERR("\nLoading image failed");
  return kStatus_Fail;
}

/*******************************************************************************
 * Externs
 ******************************************************************************/
/*
 *                                 |
 * +-------------------------------|----------------|------------------|
 * |         |                   |T|              |T|                |T|
 * |         |                   |R|              |R|                |R|
 * |         |                   |A|              |A|                |A|
 * | MCUBOOT |    ACTIVE SLOT    |I| PRIMARY SLOT |I| SECONDARY SLOT |I|
 * |         |                   |L|              |L|                |L|
 * |         |                   |E|              |E|                |E|
 * |         |                   |R|              |R|                |R|
 * +-------------------------------|----------------|------------------|
 *            on-the-fly decrypted |          staging area
 *            execution slot       |          direct-xip mode
 *            platform specific    |   driven and encrypted by mcuboot
 * 
 * Trailers (metadata) have reserved area of sector size at the end of the flash
 * area. The trailer of encrypted slot is platform specific and used for 
 * particular target encryption handling.
 */     

/**
Function analyzes encrypted image referenced by MCUBoot response object and 
compares it with an image in execution slot. If the content of execution slot is 
invalid the image from staged slot is re-encrypted into execution slot with new 
encryption context.
MCUBoot response object is then modified so bootloader can jump to execute the 
main image in execution slot.

\param rsp pointer to boot_rsp response object

\return SDK Error Code, use kStatus_Success or kStatus_Fail to evaluate.
*/
status_t mcuboot_process_encryption(struct boot_rsp *rsp)
{
  enc_data_t enc_data;
  uint32_t tlv_off;
  uint32_t active_slot;
  status_t status;
  uint32_t nonce[16 / sizeof(uint32_t)];
   
  bootutil_aes_ctr_init(&enc_data.ctx);
  
  /* Extracts referenced image info */
  if(rsp->br_image_off == PRIMARY_SLOT_OFFSET)
    enc_data.slot = 0;
  else
    enc_data.slot = 1;
  BOOT_LOG_INF("Referenced image is located in the %s slot",
                     (enc_data.slot == 0) ? "primary" : "secondary");
  
  enc_data.fap_src = &boot_flash_map[enc_data.slot];
  enc_data.fap_dst = &boot_enc_flash_map[0];
  
  if(flash_area_read(enc_data.fap_src, 0, (void *)&enc_data.hdr, 
                     sizeof(struct image_header)) != 0)
  {
    BOOT_LOG_INF("Flash read failed");
    goto error;
  }
  
  if(memcmp(&enc_data.hdr, rsp->br_hdr, sizeof(struct image_header)) != 0)
    goto error;
  
  tlv_off = enc_data.hdr.ih_hdr_size + enc_data.hdr.ih_img_size;
  if(flash_area_read(enc_data.fap_src, tlv_off, (void *)&enc_data.tlv_info, 
                     sizeof(struct image_tlv_info)) != 0)
  {
    BOOT_LOG_INF("Flash read failed");
    goto error;
  }
  
  if (enc_data.tlv_info.it_magic != IMAGE_TLV_INFO_MAGIC)
    goto error;
  
  //BOOT_LOG_INF("Analyzed mcuboot image...");
  //printf_enc_data(&enc_data);
  
  /* Decrypt and load AES-CTR key located in TLV of referenced image */
  BOOT_LOG_INF("Decrypting and loading MCUBOOT AES-CTR key for staged image...");
  if(find_and_load_aes_key(&enc_data) != kStatus_Success)
  {
    BOOT_LOG_INF("Loading mcuboot AES-CTR key failed");
    goto error;
  }
  
  /* Check content in execution slot and eventually validate it against reference */
  BOOT_LOG_INF("Checking execution slot...");
  status = platform_enc_check_slot(boot_enc_flash_map, &active_slot);
  if(status == kStatus_Success)
  {
    BOOT_LOG_INF("An image in execution slot was found");
    BOOT_LOG_INF("Comparing the image against reference chosen by MCUBoot...");
    if(active_slot == enc_data.slot)
    {  
      if(platform_enc_init_slot(boot_enc_flash_map, (uint8_t *)nonce) == kStatus_Success){
        if(verify_image(&enc_data) == kStatus_Success){
          /* The image in execution slot is valid */
          BOOT_LOG_INF("The image is valid");
          goto finish;
        }
      }
      BOOT_LOG_INF("Image in execution slot is invalid");
    }
    platform_enc_deinit_slot(boot_enc_flash_map);
  }
  BOOT_LOG_INF("No valid image found in staging area...");

  /* Install new image */
  BOOT_LOG_INF("Preparing execution slot for new image");
  
  if(platform_enc_prepare_slot(boot_enc_flash_map, enc_data.slot) != kStatus_Success)
    goto error;
  
  BOOT_LOG_INF("Installing new image into execution slot from staged area");

  /* Prepare on-the-fly decryption based on configuration */
  if(platform_enc_init_slot(boot_enc_flash_map, (uint8_t *)nonce) != kStatus_Success)
    goto error;
  /* Re-encrypt image from staging slot to execution slot */
  if(load_image(&enc_data, (uint8_t *)nonce) != kStatus_Success)
    goto error;
  /* Verify new installed image against staged reference */
  if(verify_image(&enc_data) != kStatus_Success)
    goto error;

finish:
  bootutil_aes_ctr_drop(&enc_data.ctx);
  rsp->br_flash_dev_id = enc_data.fap_dst->fa_device_id;
  rsp->br_image_off = ENCRYPTED_SLOT_OFFSET;
  /* Invalidate caches */
  //SCB_DisableDCache();
  //SCB_DisableICache();
  return kStatus_Success;
  
error:
  bootutil_aes_ctr_drop(&enc_data.ctx);
  //BOOT_LOG_INF("Dumping...");
  //dump_image();
  return kStatus_Fail;
}

#endif /* CONFIG_MCUBOOT_ENCRYPTED_XIP_SUPPORT */