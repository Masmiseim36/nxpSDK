/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sb3_api.h"
#include "fsl_debug_console.h"
#include "fsl_romapi_iap.h"
#include "mflash_drv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ROM_BUF_SIZE     0x6000

#define FLASH_REMAP_REG  0x40134428           /* RW61x flash remap offset register */
#define FLASH_BASE       0x08000000
#define FLASH_FCB_OFFSET (FLASH_BASE + 0x400) /* FCB of main app  */

#define OTP_RKTH_383_352_FUSE_IDX     (104u)
#define OTP_RKTH_383_352_FUSE_VALUE() (OCOTP->OTP_SHADOW[OTP_RKTH_383_352_FUSE_IDX])

#define OTP_CUST_SK_MK_31_0_FUSE_IDX     (92u)
#define OTP_CUST_SK_MK_31_0_FUSE_VALUE() (OCOTP->OTP_SHADOW[OTP_CUST_SK_MK_31_0_FUSE_IDX])

#define OTP_BOOT_CFG2_FUSE_IDX           (17u)
#define OTP_BOOT_CFG2_FUSE_VALUE()       (OCOTP->OTP_SHADOW[OTP_BOOT_CFG2_FUSE_IDX])

typedef struct
{
    api_core_context_t core_ctx;
    kp_api_init_param_t init_param;
} sb3_api_ctx_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifndef SDK_OS_FREE_RTOS
static uint32_t static_buf[ROM_BUF_SIZE / sizeof(uint32_t)];
#endif

/*******************************************************************************
 * Static
 ******************************************************************************/
sb3_api_ctx_t sb3_api_ctx;

static int is_sb3_header(const void *header)
{
    return !memcmp("sbv3", header, 4);
}

/* Note: there is no clear way how to detect mbi presence so we analyze atleast Image Type */
static int mbi_image_info_check(const uint32_t *image)
{
    uint32_t image_type = image[0x24/4];
    uint32_t load_addr  = image[0x34/4];

    //Image type <= 0x8
    if((image_type & 0x3F) > 0x8)
        return 0;
    //Image Subtype <= 0x1
    if((image_type>>6 & 0x3) > 0x1)
        return 0;
    if(load_addr == 0xFFFFFFFF)
        return 0;
    return 1;
}

static void mbi_image_info_parse(const uint32_t *image, struct mbi_image_info *info)
{
    info->length       = image[0x20/4];
    info->type         = image[0x24/4] & 0xff;
    info->img_version  = (image[0x24/4] & (1<<10)) ? (image[0x24/4] >> 16) : 0;
    info->execaddr     = image[0x34/4];

    info->cert_offset = image[0x28/4];
    //info->cert_size   = image[info->cert_offset/4 + 2];
    //info->fw_version  = image[(info->cert_offset+info->cert_size)/4 + 2];
}
/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * Externs
 ******************************************************************************/

int is_remap_active(void)
{
    return (*((volatile uint32_t *)FLASH_REMAP_REG) > 0) ? 1 : 0;
}

int sb3_parse_header(const void *header, uint32_t *sb3_len)
{
    if(!is_sb3_header(header))
    {
        return 0;
    }
    if(sb3_len != NULL)
    {
        /* Calculate SB3 file size */
        /* blockCount * blockSize + imageTotalLength */
        uint32_t *sb3_ptr = (uint32_t *) header;
        *sb3_len = sb3_ptr[0xC/4] * sb3_ptr[0x10/4] + sb3_ptr[0x20/4];
    }
    return 1;
}

void mbi_print_info(void)
{
    struct mbi_image_info mbi_info;
    status_t status;

    uint32_t img_ver_addr[2];
    uint32_t img_mbi_addr[2];
    uint32_t image1_off;
    int remap_active;
    uint32_t mbi_buf[64/sizeof(uint32_t)];
    
    
    if(OTP_BOOT_CFG2_FUSE_VALUE() != 0)
    {
        image1_off = (OTP_BOOT_CFG2_FUSE_VALUE() >> 11 & 0x3FF) * 256 * 1024;
    }
    else
    {
        PRINTF("Warning: Fuse/shadow BOOT_CFG2 is not set. The image 1 offset can be invalid\n");
        image1_off = 0x400000;
    }

    img_ver_addr[0] = 0x0600;
    img_mbi_addr[0] = 0x1000;
    img_ver_addr[1] = image1_off + 0x0600;
    img_mbi_addr[1] = image1_off + 0x1000;

    if (is_remap_active())
    {
        PRINTF("Flash REMAP_OVERLAY active\n");
        remap_active = 1;
    }
    else
    {
        PRINTF("Flash REMAP_OVERLAY disabled\n");
        remap_active = 0;
    }

    for (int img_index = 0; img_index < 2; img_index++)
    {
        status = mflash_drv_read(img_mbi_addr[img_index], mbi_buf, 64);
        if(status != kStatus_Success)
        {
            PRINTF("mflash_drv_read failed\n");
            return;
        }
        if (mbi_image_info_check((void *) mbi_buf))
        {
            uint32_t img_ver;

            /* in case of RW61x the image version is different from value in mbi header */
            status = mflash_drv_read(img_ver_addr[img_index], &img_ver, 4);
            if(status != kStatus_Success)
            {
                PRINTF("mflash_drv_read failed\n");
                return;
            }

            mbi_image_info_parse(mbi_buf, &mbi_info);

            PRINTF("IMAGE %u:\n", img_index);
            PRINTF("    <IMG_VERSION 0x%X LENGTH %u EXEC_ADDR 0x%X>\n",
                    img_ver & 0xFFFF,
                    mbi_info.length,
                    mbi_info.execaddr);
            if((img_index == 0 && remap_active == 0) || (img_index == 1 && remap_active == 1))
            {
                PRINTF("    *ACTIVE*\n");
            }
        }
        else
        {
            PRINTF("IMAGE %u: Invalid image header\n", img_index);
        }
    }
}

int sb3_check_provisioning(bool rom_only)
{
    if(OTP_RKTH_383_352_FUSE_VALUE() != 0 && OTP_CUST_SK_MK_31_0_FUSE_VALUE() != 0)
    {
        return 1;
    }
    return 0;
}

status_t sb3_api_init(void)
{
    status_t status;
    sb3_api_ctx_t *ctx = &sb3_api_ctx;

    const size_t iapWorkBufSize = ROM_BUF_SIZE;

    //standard_version_t iapVersion = iap_api_version();
    //PRINTF("IAP API version=%d.%d.%d\n", iapVersion.major, iapVersion.minor, iapVersion.bugfix);

    memset(&ctx->core_ctx, 0x0, sizeof(ctx->core_ctx));

    memset(&ctx->init_param, 0x0, sizeof(ctx->init_param));

    ctx->init_param.allocSize = iapWorkBufSize;
#ifndef SDK_OS_FREE_RTOS
    ctx->init_param.allocStart = (uint32_t) static_buf;
#else
    ctx->init_param.allocStart = (uint32_t) pvPortMalloc(iapWorkBufSize);
#endif

    if (ctx->init_param.allocStart == 0)
    {
        PRINTF("%s: Failed to allocate memory for IAP work buffer\n", __func__);
        return kStatus_Fail;
    }

    standard_version_t iapVersion = (standard_version_t) iap_api_version();
    PRINTF("IAP API version=%d.%d.%d\n", iapVersion.major, iapVersion.minor, iapVersion.bugfix);

    status = iap_api_init(&(ctx->core_ctx), &(ctx->init_param));
    if (status != kStatus_Success)
    {
        PRINTF("%s: iap_api_init() failed with %d\n", __func__, status);
        goto cleanup;
    }

    /* Let ROM IAP configure flash driver from FCB of SBL */
#if 0
    status = iap_mem_config(&(ctx->core_ctx), (uint32_t *) FLASH_FCB_OFFSET, kMemoryID_FlexspiNor);
    if(status != kStatus_Success) {
        PRINTF("iap_mem_config returned with code 0x%X\n", status);
        return status;
    }
#endif

    status = iap_sbloader_init(&ctx->core_ctx);
    if (status != kStatus_Success)
    {
        PRINTF("%s: iap_sbloader_init() failed with %d\n", __func__, status);
        goto cleanup;
    }

    return status;

cleanup:
#ifdef SDK_OS_FREE_RTOS
    vPortFree((void *) ctx->init_param.allocStart);
#endif

    return status;
}

status_t sb3_api_pump(uint8_t *data, size_t len)
{
    status_t status;
    sb3_api_ctx_t *ctx = &sb3_api_ctx;

    status = iap_sbloader_pump(&ctx->core_ctx, data, len);

    if (status != kStatus_Success && status != kStatusRomLdrDataUnderrun)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}


void sb3_api_finalize(void)
{
    sb3_api_ctx_t *ctx = &sb3_api_ctx;
    iap_sbloader_finalize(&ctx->core_ctx);
}


status_t sb3_api_deinit(void)
{
    sb3_api_ctx_t *ctx = &sb3_api_ctx;
#ifdef SDK_OS_FREE_RTOS
    vPortFree((void *)ctx->init_param.allocStart);
#endif
    return iap_api_deinit(&ctx->core_ctx);
}
