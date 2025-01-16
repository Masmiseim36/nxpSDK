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
#if defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE) && !defined(CONFIG_ENCRYPT_XIP_EXT_OVERWRITE_ONLY)

#include <stdint.h>
#include <ctype.h>

#include "fsl_cache.h"
#include "fsl_debug_console.h"
#include "encrypted_xip_mcuboot_support.h"
#include "encrypted_xip.h"
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
/*
 * @brief Helper to assert function return status.
 *
 * @details Print finished message and trap forever upon failure.
 */
#define ASSERT_ENC(expected, actual, ...) \
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

#if (BOOT_FLASH_ACT_APP % MFLASH_SECTOR_SIZE != 0) || \
    (BOOT_FLASH_CAND_APP % MFLASH_SECTOR_SIZE != 0)|| \
    (BOOT_FLASH_EXEC_APP % MFLASH_SECTOR_SIZE != 0)
#error "Incorrect aligment of image slots"
#endif

#define PRIMARY_SLOT_OFFSET     (BOOT_FLASH_ACT_APP - BOOT_FLASH_BASE)
#define SECONDARY_SLOT_OFFSET   (BOOT_FLASH_CAND_APP - BOOT_FLASH_BASE)
#define ENCRYPTED_SLOT_OFFSET   (BOOT_FLASH_EXEC_APP - BOOT_FLASH_BASE)

#define TLV_KEY_LEN             256
#define TLV_SHA256_LEN          32

#define ENC_MAGIC_SZ            16                 

/* IPED based on GCM requires writes to IPED regions are always a multiple of 4 pages*/
#if defined(ENCRYPTED_XIP_IPED)
#define FLASH_WRITE_SIZE     (MFLASH_PAGE_SIZE*4)
#else
#define FLASH_WRITE_SIZE     1024
#endif
      
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
} enc_data_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Static
 ******************************************************************************/
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

static void printf_enc_data(enc_data_t *data) 
{
	BOOT_LOG_INF("\nSource image \nflash device id 0x%X \nflash id 0x%X \noffset 0x%X"
                     "\nsize %d bytes", data->fap_src->fa_device_id,
			data->fap_src->fa_id, data->fap_src->fa_off, data->fap_src->fa_size);
	BOOT_LOG_INF("header size 0x%X", data->hdr.ih_hdr_size);
	BOOT_LOG_INF("img size %d bytes", data->hdr.ih_img_size);
	BOOT_LOG_INF("protected tlv size %d bytes", data->hdr.ih_protect_tlv_size);
	BOOT_LOG_INF("tlv tot size %d bytes", data->tlv_info.it_tlv_tot);

	BOOT_LOG_INF("\nDestination encrypted image \nflash device id 0x%X \nflash id 0x%X"
		     "\noffset 0x%X \nsize %d bytes",
                      data->fap_src->fa_device_id, data->fap_dst->fa_id,
                      data->fap_dst->fa_off, data->fap_dst->fa_size);
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
        
        /* sz = 0 nothing to encrypt when skipping over the TLVs. */
        if (sz == 0) {
            return kStatus_Success;
        }

	memset(nonce, 0, 12);
	off_nonce >>= 4;
	nonce[12] = (uint8_t) (off_nonce >> 24);
	nonce[13] = (uint8_t) (off_nonce >> 16);
	nonce[14] = (uint8_t) (off_nonce >> 8);
	nonce[15] = (uint8_t) off_nonce;

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
	if (bootutil_tlv_iter_begin(&it, hdr, fap, IMAGE_TLV_ENC_RSA2048, false))
            goto error;
	    
	if (bootutil_tlv_iter_next(&it, &tlv_offset, &tlv_len, NULL)){
            BOOT_LOG_INF("No key encrypted with RSA-OAEP-2048 found...");
            goto error;
        }
	if (tlv_len != BOOT_ENC_TLV_SIZE)
	    goto error;

	if (flash_area_read(fap, tlv_offset, tlv_enc_key, BOOT_ENC_TLV_SIZE) != 0) {
	    BOOT_LOG_INF("Flash read failed");
	    goto error;
	}

	/* Decrypt AES-CTR key and load it */
	if (boot_enc_decrypt(tlv_enc_key, dec_aes_key) != 0) {
	    BOOT_LOG_INF("Failed to decrycpt AES key in TLV section");
	    goto error;
	}

	if (bootutil_aes_ctr_set_key(&data->ctx, dec_aes_key) != 0)
	    goto error;

	BOOT_LOG_INF("AES-CTR key loaded");
	return kStatus_Success;
	error: return kStatus_Fail;
}

#define TMPBUF_SZ       256

/**
 * Function verifies encrypted image in execution slot by comparing hash of refence 
 * image and calculated hash of the verified image.
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
	if (bootutil_tlv_iter_begin(&it, hdr, fap_src, IMAGE_TLV_SHA256, false))
		goto error;
	if (bootutil_tlv_iter_next(&it, &tlv_offset, &tlv_len, NULL))
		goto error;
	if (tlv_len != TLV_SHA256_LEN)
		goto error;
	if (flash_area_read(fap_src, tlv_offset, hash_src, TLV_SHA256_LEN) != 0) {
		BOOT_LOG_INF("Flash read failed");
		goto error;
	}

	/* Compute hash of re-encrypted image in execution slot */
        /* Hash is computed over image header, payload and protected TLV */
	bootutil_sha_init(&sha256_ctx);
	size = hdr->ih_hdr_size + hdr->ih_img_size + hdr->ih_protect_tlv_size;
	for (off = 0; off < size; off += blk_sz) {
		blk_sz = size - off;
		if (blk_sz > TMPBUF_SZ) {
			blk_sz = TMPBUF_SZ;
		}
		if (flash_area_read(fap_dst, off, tmp_buf, blk_sz) != 0) {
			BOOT_LOG_INF("Flash read failed");
			goto error;
		}
		bootutil_sha_update(&sha256_ctx, tmp_buf, blk_sz);
	}
	bootutil_sha_finish(&sha256_ctx, (uint8_t*) hash_dst);

	/* Compare hash of reference image with re-encrypted image */
	if (memcmp(hash_dst, hash_src, TLV_SHA256_LEN) != 0)
		goto error;

	BOOT_LOG_INF("Image verification successful");
	bootutil_sha_drop(&sha256_ctx);
	return kStatus_Success;
	error: bootutil_sha_drop(&sha256_ctx);
	BOOT_LOG_ERR("Image verification failed");
	return kStatus_Fail;
}

#define CHUNK_SIZE      FLASH_WRITE_SIZE

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
    static uint32_t buf[CHUNK_SIZE / sizeof(uint32_t)];
    const struct flash_area *fap_src = data->fap_src;
    const struct flash_area *fap_dst = data->fap_dst;
    const uint32_t hdr_size = data->hdr.ih_hdr_size;
    struct image_header *hdr = &data->hdr;
    const uint32_t tlv_off = hdr_size + data->hdr.ih_img_size;
    const uint32_t sz = tlv_off + data->hdr.ih_protect_tlv_size
			+ data->tlv_info.it_tlv_tot;
    uint32_t bytes_copied;
    uint32_t chunk_sz;
    uint32_t blk_sz;
    uint32_t blk_off;
    uint16_t idx;
    uint32_t abs_off;
    uint32_t flash_dst_addr = BOOT_FLASH_EXEC_APP;
    int rc;

#if defined(ENCRYPTED_XIP_IPED)
    /* Speed up the erase process without erasing whole slot */
    const uint32_t slotsize = fap_dst->fa_size;
    const uint32_t page_align = 4*MFLASH_PAGE_SIZE;
    const uint32_t max_slotcnt = (slotsize - 1 + MFLASH_SECTOR_SIZE) / MFLASH_SECTOR_SIZE;
    uint32_t slotaddr = fap_dst->fa_off;
    
    //calculate number of slots to erase
    //align up to 4*page size
    uint32_t tmp = sz + (sz % page_align == 0 ? 0 : (page_align - sz % page_align));
    //1.25x
    tmp = tmp * 5 / 4;
    uint32_t slotcnt = (tmp + (tmp % MFLASH_SECTOR_SIZE == 0 ? 0 : (MFLASH_SECTOR_SIZE - tmp % MFLASH_SECTOR_SIZE))) / MFLASH_SECTOR_SIZE;
    if(slotcnt >= max_slotcnt){
        BOOT_LOG_ERR("Final size of encrypted image exceeds slot size!");
        goto error;
    }
    BOOT_LOG_INF("Erasing the execution slot...");
    
    for (int i = 0; i < slotcnt; i++) {
	if (mflash_drv_sector_erase(slotaddr) != kStatus_Success) {
	    BOOT_LOG_ERR("Failed to erase sector at 0x%X\r\n", slotaddr);
	    goto error;;
        }
        if (i % 4 == 0)
            PUTCHAR('.');
        slotaddr += MFLASH_SECTOR_SIZE;
    }
#endif /* ENCRYPTED_XIP_IPED */
    
    BOOT_LOG_INF("\nRe-encrypting staged image to execution slot...");
    /* following code is adjusted boot_copy_region function from mcuboot */
    bytes_copied = 0;
    while (bytes_copied < sz) {
        if (sz - bytes_copied > sizeof buf) {
            chunk_sz = sizeof buf;
        } else {
            chunk_sz = sz - bytes_copied;
        }

        rc = flash_area_read(fap_src, bytes_copied, buf, chunk_sz);
        if (rc != 0) {
            goto error;
        }

        /* assume the secondary slot as src, needs decryption */
        /* only payload is encrypted in source image */
        abs_off = bytes_copied;
        if (abs_off < hdr->ih_hdr_size) {
            /* do not decrypt header */
            if (abs_off + chunk_sz > hdr->ih_hdr_size) {
                /* The lower part of the chunk contains header data */
                blk_off = 0;
                blk_sz = chunk_sz - (hdr->ih_hdr_size - abs_off);
                idx = hdr->ih_hdr_size  - abs_off;
            } else {
                /* The chunk contains exclusively header data */
                blk_sz = 0; /* nothing to decrypt */
                }
        } else {
            idx = 0;
            blk_sz = chunk_sz;
            blk_off = (abs_off - hdr->ih_hdr_size) & 0xf;
        }

        if (blk_sz > 0) {
            if (abs_off + chunk_sz > tlv_off) {
                /* do not decrypt TLVs */
                if (abs_off >= tlv_off) {
                    blk_sz = 0;
                } else {
                    blk_sz = tlv_off - abs_off;
                }
            }
            rc = decrypt(&data->ctx, (abs_off + idx) - hdr->ih_hdr_size, 
                         blk_sz, blk_off, (uint8_t*) &buf[idx]);
            if (rc != 0) {
                BOOT_LOG_INF("Decryption failed");
                goto error;
            }
        }
#if !defined(ENCRYPTED_XIP_IPED)
        /* Number of sectors differs in case of IPED */
        if ((abs_off % MFLASH_SECTOR_SIZE) == 0) {
            rc = flash_area_erase(fap_dst, abs_off, MFLASH_SECTOR_SIZE);
            if (rc != 0) {
                BOOT_LOG_INF("Flash erase failed");
                goto error;
            }
        }
#endif
        rc = encrypted_xip_encrypt_data(flash_dst_addr + abs_off, nonce, 
                                       (uint8_t*) buf, (uint8_t*) buf, chunk_sz);
	if (rc != 0) {
            BOOT_LOG_INF("Encryption failed");
            goto error;
	}
        uint32_t chunk_sz_tmp;
#if defined(ENCRYPTED_XIP_IPED)
        /* If execution is also done with XIP from an IPED regions, the size of 
         * the written data MUST be a multiple of 4 pages.
         */
        if(chunk_sz < CHUNK_SIZE){
            chunk_sz_tmp = CHUNK_SIZE;
        }else{
            chunk_sz_tmp = chunk_sz;
        }
#else
        chunk_sz_tmp = chunk_sz;
#endif
        rc = encrypted_xip_flash_write(fap_dst, bytes_copied, buf, chunk_sz_tmp);
        if (bytes_copied % (MFLASH_SECTOR_SIZE*4) == 0)
            PUTCHAR('.');
        if (rc != 0) {
            BOOT_LOG_INF("Flash write failed");
            goto error;
        }

        bytes_copied += chunk_sz;
    }
        
    //BOOT_LOG_INF("\nbytes_copied %d", bytes_copied);
    //BOOT_LOG_INF("chunk_sz %d", chunk_sz);
    //BOOT_LOG_INF("blk_sz %d", blk_sz);
    //BOOT_LOG_INF("blk_off %d", blk_off);
    //BOOT_LOG_INF("abs_off %d", abs_off);
        
    if (bytes_copied != sz)
        goto error;

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
 *            platform specific    |       encrypted by mcuboot
 * 
 * Trailers ave reserved area of sector size at the end of the flash area. 
 * The trailer of encrypted slot is platform specific and used for 
 * particular target encryption handling.
 */

/**
 Function analyzes an encrypted image referenced by MCUBoot response object and
 compares it with the image in execution slot. If the content of execution slot is
 invalid then the image from staged slot is re-encrypted into execution slot with
 new encryption configuration to ensure different ciphertext per update.
 If success the response object is adjusted with the address of execution slot .

 \param rsp pointer to boot_rsp response object

 \return SDK Error Code, use kStatus_Success or kStatus_Fail to evaluate.
 */
status_t encrypted_xip_process(struct boot_rsp *rsp) 
{
	enc_data_t enc_data;
	uint32_t tlv_off;
	uint32_t active_slot;
	status_t status;
	uint32_t nonce[16 / sizeof(uint32_t)];

	bootutil_aes_ctr_init(&enc_data.ctx);
        
        status = encrypted_xip_init();
        if(status != kStatus_Success){
            BOOT_LOG_INF("encrypted_xip_init failed");
            goto clean;
        }

	/* Extracts referenced image info */
	if (rsp->br_image_off == PRIMARY_SLOT_OFFSET)
		enc_data.slot = 0;
	else
		enc_data.slot = 1;
	BOOT_LOG_INF("Referenced image is located in the %s slot",
			(enc_data.slot == 0) ? "primary" : "secondary");

	enc_data.fap_src = &boot_flash_map[enc_data.slot];
	enc_data.fap_dst = &boot_flash_exec_map[0];

	if (flash_area_read(enc_data.fap_src, 0, (void*) &enc_data.hdr,
			sizeof(struct image_header)) != 0) {
		BOOT_LOG_INF("Flash read failed");
		goto clean;
	}

	if (memcmp(&enc_data.hdr, rsp->br_hdr, sizeof(struct image_header)) != 0)
		goto clean;

	tlv_off = enc_data.hdr.ih_hdr_size + enc_data.hdr.ih_img_size;
	if (flash_area_read(enc_data.fap_src, 
                            tlv_off, 
                            (void*) &enc_data.tlv_info,
			    sizeof(struct image_tlv_info)) != 0) {
		BOOT_LOG_INF("Flash read failed");
		goto clean;
	}

	if (enc_data.tlv_info.it_magic != IMAGE_TLV_INFO_MAGIC)
		goto clean;

	/* Decrypt and load AES-CTR key located in TLV of referenced image */
	BOOT_LOG_INF("Decrypting and loading the MCUBOOT AES-CTR key for staged image...");
        status = find_and_load_aes_key(&enc_data);
	if (status != kStatus_Success) {
	    BOOT_LOG_INF("Failed to load AES-CTR key");
	    goto clean;
	}

	/* Check content in execution slot and eventually validate it against reference */
	BOOT_LOG_INF("Checking the execution slot...");
        bool cfg_valid;
	status = encrypted_xip_cfg_check(boot_flash_meta_map, &cfg_valid, &active_slot);
        if (status != kStatus_Success) {
            BOOT_LOG_INF("encrypted_xip_cfg_check failed");
            goto clean;
        }
	if (cfg_valid == true) {
            BOOT_LOG_INF("An image in execution slot was found");
            BOOT_LOG_INF("Comparing the image against reference...");
            if (active_slot == enc_data.slot) {
                /* Expected slot number matches, initialize encryption unit */
                status = encrypted_xip_cfg_initEncryption(boot_flash_meta_map);
                if (status == kStatus_Success) {
                    status = verify_image(&enc_data);
                    if (status == kStatus_Success) {
                        /* The image in execution slot is valid */
			BOOT_LOG_INF("The image is valid");
                        goto success;
                    }
                }
		BOOT_LOG_INF("The image in execution slot is invalid or doesn't match the reference");
            }
	}
	BOOT_LOG_INF("No valid image found in staging area...");

	/* Install new image with new encryption metadata */
	BOOT_LOG_INF("Preparing execution slot for new image");

        status = encrypted_xip_cfg_write(boot_flash_meta_map);
	if (status != kStatus_Success){
            BOOT_LOG_INF("encrypted_xip_cfg_write failed");
            goto clean;
        }
	/* Prepare on-the-fly decryption based on configuration */
        status = encrypted_xip_cfg_initEncryption(boot_flash_meta_map);
        if (status != kStatus_Success) {
            BOOT_LOG_INF("encrypted_xip_cfg_initEncryption failed");
            goto clean;
        }
        status = encrypted_xip_cfg_getNonce(boot_flash_meta_map, (uint8_t*) nonce);
	if (status != kStatus_Success){
            BOOT_LOG_INF("encrypted_xip_cfg_initEncryption failed");  
            goto clean;
        }
        BOOT_LOG_INF("Installing new image into execution slot from staged area...");
	/* Re-encrypt image from staging slot to execution slot */
        status = load_image(&enc_data, (uint8_t*) nonce);
	if (status != kStatus_Success){
            BOOT_LOG_INF("load_image failed");  
            goto clean;
        }
	/* Verify new installed image against staged reference */
        status = verify_image(&enc_data);
	if (status != kStatus_Success){
            BOOT_LOG_INF("verify_image failed");  
            goto clean;
        }
        
        status = encrypted_xip_cfg_confirm(boot_flash_meta_map, enc_data.slot);
        if (status != kStatus_Success){
            BOOT_LOG_INF("encrypted_xip_cfg_confirm failed");  
            goto clean;
        }

        success:
        rsp->br_flash_dev_id = enc_data.fap_dst->fa_device_id;
	rsp->br_image_off = ENCRYPTED_SLOT_OFFSET;
	clean: 
        bootutil_aes_ctr_drop(&enc_data.ctx);
        encrypted_xip_finish();
	return status;
}

#endif /* CONFIG_ENCRYPT_XIP_EXT_ENABLE */
