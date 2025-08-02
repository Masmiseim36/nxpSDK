/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sb3_api.h"
#include "fsl_flash.h"
#include "fsl_debug_console.h"
#include "fsl_mem_interface.h"
#include "fsl_sbloader.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ROM_BUF_SIZE        0x1000

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
/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * Externs
 ******************************************************************************/

int is_remap_active(void)
{
    return NPX0->REMAP ? 1 : 0;
}

void mbi_print_info(void)
{
    struct mbi_image_info imginfo;

    uint32_t img_mbi_addr[2];
    int remap_active;

    if (is_remap_active())
    {
        PRINTF("Flash REMAP_SWAP active\n");
        remap_active = 1;
        img_mbi_addr[0] = 0x100000;
        img_mbi_addr[1] = 0;
    }
    else
    {
        PRINTF("Flash REMAP_SWAP disabled\n");
        remap_active = 0;
        img_mbi_addr[0] = 0;
        img_mbi_addr[1] = 0x100000;
    }

    for (int img_index = 0; img_index < 2; img_index++)
    {
        if (mbi_image_info_check((void *) img_mbi_addr[img_index]))
        {
            mbi_image_info_parse((uint32_t *) img_mbi_addr[img_index], &imginfo);

            PRINTF("IMAGE %u:\n", img_index);
            PRINTF("    <IMG_VERSION 0x%X LENGTH %u EXEC_ADDR 0x%X>\n",
                    imginfo.img_version,
                    imginfo.length,
                    imginfo.execaddr);
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

/* Note: there is no clear way how to detect mbi presence so we analyze atleast Image Type */
int mbi_image_info_check(const uint32_t *image)
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

int sb3_check_provisioning(bool rom_only)
{
    int cmpa_ok;
    int remap_size;

    /* Sanity check - test device is setup - CMPA header is present */

    cmpa_ok = *((uint16_t*)0x1004002) == 0x5963;
    remap_size = ((*((uint8_t *)0x1004004) & 0x1F)+1)*32;

    if (!cmpa_ok || (rom_only == true && remap_size == 0))
    {
        PRINTF("CMPA header %s\n", cmpa_ok ? "OK" : "MISMATCH!");
        if(rom_only == true)
            PRINTF("REMAP size %u kB\n", remap_size);
        return 0;
    }
    return 1;
}

void mbi_image_info_parse(const uint32_t *image, struct mbi_image_info *info)
{
    info->length       = image[0x20/4];
    info->type         = image[0x24/4] & 0xff;
    info->img_version  = (image[0x24/4] & (1<<10)) ? (image[0x24/4] >> 16) : 0;
    info->execaddr     = image[0x34/4];

    info->cert_offset = image[0x28/4];
    info->cert_size   = image[info->cert_offset/4 + 2];
    info->fw_version  = image[(info->cert_offset+info->cert_size)/4 + 2];
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

status_t sb3_api_init(void)
{
    status_t status;
    sb3_api_ctx_t *ctx = &sb3_api_ctx;

    const size_t iapWorkBufSize = ROM_BUF_SIZE;

    //standard_version_t iapVersion = iap_api_version();
    //PRINTF("IAP API version=%d.%d.%d\n", iapVersion.major, iapVersion.minor, iapVersion.bugfix);

    memset(&ctx->core_ctx, 0x0, sizeof(ctx->core_ctx));

    ctx->core_ctx.flashConfig.modeConfig.sysFreqInMHz = SystemCoreClock / 1000000;

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

    status = API_Init(&(ctx->core_ctx), &(ctx->init_param));
    if (status != kStatus_Success)
    {
        PRINTF("%s: API_Init() failed with %d\n", __func__, status);
        goto cleanup;
    }

    status = Sbloader_Init(&ctx->core_ctx);
    if (status != kStatus_Success)
    {
        PRINTF("%s: Sbloader_Init() failed with %d\n", __func__, status);
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

    status = Sbloader_Pump(&ctx->core_ctx, data, len);

    if (status != kStatus_Success && status != kStatusRomLdrDataUnderrun)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}


void sb3_api_finalize(void)
{
    sb3_api_ctx_t *ctx = &sb3_api_ctx;
    Sbloader_Finalize(&ctx->core_ctx);
}


status_t sb3_api_deinit(void)
{
    sb3_api_ctx_t *ctx = &sb3_api_ctx;
#ifdef SDK_OS_FREE_RTOS
    vPortFree((void *)ctx->init_param.allocStart);
#endif
    return API_Deinit(&ctx->core_ctx);
}
