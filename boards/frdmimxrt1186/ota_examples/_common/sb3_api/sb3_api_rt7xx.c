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
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ROM_BUF_SIZE     0x6000

#define FLASH_REMAP_REG  0x40030000           /* RT700 MMU0 register to detect flash remap */
#define FLASH_BASE       0x28000000
#define FLASH_FCB_OFFSET (FLASH_BASE)         /* FCB of main app  */

typedef struct 
{   
    __IO uint32_t OTP_SHADOW[512];                   
} OCOTP_RT700_SHADOWS_Type;
#define OCOTP_RT700_SHADOWS ((OCOTP_RT700_SHADOWS_Type *)OCOTP_BASE)

#define  OTP_RTKH0_SHADOW_IDX         (88u)
#define  OTP_RKTH0_SHADOW_VALUE()     (OCOTP_RT700_SHADOWS->OTP_SHADOW[OTP_RTKH0_SHADOW_IDX])
#define  OTP_CUST0_SHADOW_IDX         (100u)
#define  OTP_CUST0_SHADOW_VALUE()     (OCOTP_RT700_SHADOWS->OTP_SHADOW[OTP_CUST0_SHADOW_IDX])
#define  OTP_XSPI_CFG1_SHADOW_IDX     (153u)
#define  OTP_XSPI_CFG1_SHADOW_VALUE() (OCOTP_RT700_SHADOWS->OTP_SHADOW[OTP_XSPI_CFG1_SHADOW_IDX])

/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifndef SDK_OS_FREE_RTOS
uint32_t static_buf[ROM_BUF_SIZE / sizeof(uint32_t)];
#endif

/*******************************************************************************
 * Static
 ******************************************************************************/

kp_api_init_param_t iap_init_param;
api_core_context_t core_ctx;

static int is_sb3_header(const void *header)
{
    return !memcmp("sbv3", header, 4);
}

/* parses basic information about MBI image */
static int mbi_image_info_parse(uint32_t slot_addr, struct mbi_image_info *info)
{
    status_t status;
    uint32_t flash_buf[64/sizeof(uint32_t)];
    uint32_t off = slot_addr;
    
    /* RT700: firmware version is used for image version comparison */
    /* for parsing we can't use logical access because of flash remap */
    status = mflash_drv_read(off, flash_buf, 64);
    if (status != kStatus_Success)
    {
        PRINTF("mflash_drv_read failed\n");
        return 0;
    }
    
    info->length       = flash_buf[0x20/4];
    info->type         = flash_buf[0x24/4] & 0xff;
    info->img_version  = (flash_buf[0x24/4] & (1<<10)) ? (flash_buf[0x24/4] >> 16) : 0;
    info->execaddr     = flash_buf[0x34/4];
    info->cert_offset = flash_buf[0x28/4];
    
    /* There is no clear way how to detect mbi presence so we do atleast sanity
     * check by analyzing Image Type and load address */
    if( ((info->type & 0x3F) > 0x8)    ||
        ((info->type >>6 & 0x3) > 0x1) ||
         (info->execaddr == 0xFFFFFFFF) )
    {
        return 0;
    }
    
    off = slot_addr + info->cert_offset;
    status = mflash_drv_read(off, flash_buf, 64);
    if (status != kStatus_Success)
    {
        PRINTF("mflash_drv_read failed\n");
        return 0;
    }
    /* check magic of cert block */
    if(flash_buf[0x0/4] != 0x72646863)
    {
        PRINTF("check magic of cert block failed\n");
        return 0;
    }
    info->cert_size = flash_buf[0x8/4];
    
    /* Point at image manifest */
    off = slot_addr + info->cert_offset + info->cert_size;
    status = mflash_drv_read(off, flash_buf, 64);
    if(status != kStatus_Success)
    {
        PRINTF("mflash_drv_read failed\n");
        return 0;
    }
    /* check magic of image manifest */
    if(flash_buf[0x0/4] != 0x6D676D69)
    {
        PRINTF("check magic of cert block failed\n");
        return 0;
    }
    info->fw_version = flash_buf[0x8/4];
    
    return 1;
}

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
    uint32_t img_mbi_addr[2];
    uint32_t image1_off;
    int remap_active;

    if(OTP_XSPI_CFG1_SHADOW_VALUE() != 0)
    {
        image1_off = (OTP_XSPI_CFG1_SHADOW_VALUE() >> 7 & 0x3FF) * 128 * 1024;
    }
    else
    {
        PRINTF("Warning: Fuse/shadow XSPI_CFG1 is not set. The image 1 offset can be invalid\n");
        image1_off = 0x400000;
    }
    img_mbi_addr[0] = 0x4000;
    img_mbi_addr[1] = image1_off;

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
        if (mbi_image_info_parse(img_mbi_addr[img_index], &mbi_info))
        {
            PRINTF("IMAGE %u:\n", img_index);
            PRINTF("    <IMG_VERSION 0x%X LENGTH %u EXEC_ADDR 0x%X>\n",
                    mbi_info.fw_version,
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
    if(OTP_CUST0_SHADOW_VALUE() == 0 ||  OTP_RKTH0_SHADOW_VALUE() == 0)
    {
        PRINTF("Error: For processing SB3, RKTH and CUST_SK_MK has to be provisioned.\n");
        return 0;
    }
    return 1;
}

status_t sb3_api_init(void)
{
    status_t status;

    api_core_context_t *ctx_ptr = &core_ctx;
    kp_api_init_param_t *iap_init_ptr = &iap_init_param;

    const size_t iapWorkBufSize = ROM_BUF_SIZE;

    memset(ctx_ptr, 0x0, sizeof(api_core_context_t));
    memset(iap_init_ptr, 0x0, sizeof(kp_api_init_param_t));
    
    /* Disable cache due a conflict in RT700 ROM */
    XCACHE_DisableCache(XCACHE0);
    XCACHE_DisableCache(XCACHE1);
    
    iap_init_ptr->allocSize = iapWorkBufSize;
#ifndef SDK_OS_FREE_RTOS
    iap_init_ptr->allocStart = (uint32_t) static_buf;
#else
    iap_init_ptr->init_param.allocStart = (uint32_t) pvPortMalloc(iapWorkBufSize);
#endif

    if (iap_init_ptr->allocStart == 0)
    {
        PRINTF("%s: Failed to allocate memory for IAP work buffer\n", __func__);
        return kStatus_Fail;
    }

    status = iap_api_init(ctx_ptr, iap_init_ptr);
    if (status != kStatus_Success)
    {
        PRINTF("%s: iap_api_init() failed with %d\n", __func__, status);
        goto cleanup;
    }

#if 0
    /* Let ROM IAP configure flash driver from FCB of SBL */
    status = iap_mem_config(&(ctx->core_ctx), (uint32_t *) FLASH_FCB_OFFSET, kMemoryID_FlexspiNor);
    if(status != kStatus_Success) {
        PRINTF("iap_mem_config returned with code 0x%X\n", status);
        return status;
    }
#endif

    status = iap_sbloader_init(ctx_ptr);
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
    api_core_context_t *ctx = &core_ctx;

    status = iap_sbloader_pump(ctx, data, len);

    if (status != kStatus_Success && status != kStatusRomLdrDataUnderrun)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}


void sb3_api_finalize(void)
{
    api_core_context_t *ctx = &core_ctx;
    iap_sbloader_finalize(ctx);
}


status_t sb3_api_deinit(void)
{
    api_core_context_t *ctx = &core_ctx;
#ifdef SDK_OS_FREE_RTOS
    vPortFree((void *)iap_init_ptr->allocStart);
#endif
    /* Enable again code & system cache */
    XCACHE_EnableCache(XCACHE0);
    XCACHE_EnableCache(XCACHE1);

    /* flush pipeline */
    __DSB();
    __ISB();
    
    /* Re-initialize mflash driver */
    if (mflash_drv_init() != kStatus_Success)
    {
        PRINTF("Failed to re-init flash driver\n");
    }
    
    return iap_api_deinit(ctx);
}
