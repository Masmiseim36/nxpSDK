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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "update.h"
#include "mcuboot_app_support.h"
#include "mcuboot_config.h"
#include "fsl_debug_console.h"
#include "flash_partitioning.h"
#include "mflash_common.h"
#include "mflash_drv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COMPONENTS_NUM MCUBOOT_IMAGE_NUMBER
#define IMAGE_SZ_MAX   BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP

/*******************************************************************************
 * Types
 ******************************************************************************/
typedef struct tfm_fwu_ctx_s {
    psa_fwu_component_info_t info;
    size_t recv_sz;
    bool initialized;
    bool in_use;  //indication of WRITING, CANDIDATE, UPDATED, REJECTED, FAILED
} fwu_ctx_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static fwu_ctx_t components_ctx[COMPONENTS_NUM];

/*******************************************************************************
 * Static
 ******************************************************************************/
static status_t flash_write(uint32_t offset, const void *data, size_t size)
{
    /* mflash expects 4B aligned buffer */
    uint32_t pagebuf[MFLASH_PAGE_SIZE / sizeof(uint32_t)];
    uint8_t *pagebuf8 = (uint8_t *)pagebuf;
    uint8_t *data8    = (void *)data;
    status_t status;

    while (size > 0)
    {
        size_t chunk = (size > sizeof(pagebuf)) ? sizeof(pagebuf) : size;

        memcpy(pagebuf, data8, chunk);

        /* In case of non-page-aligned size pad with 0xff */
        if (chunk < sizeof(pagebuf))
        {
            memset(pagebuf8 + chunk, 0xff, sizeof(pagebuf) - chunk);
        }

        status = mflash_drv_page_program(offset, pagebuf);
        if (status != kStatus_Success)
        {
            return status;
        }

        offset += chunk;
        data8 += chunk;
        size -= chunk;
    }

    return kStatus_Success;
}

static const char *state_to_str(uint8_t state)
{
    switch (state)
    {
        case PSA_FWU_READY:
            return "READY";
        case PSA_FWU_WRITING:
            return "WRITING";
        case PSA_FWU_CANDIDATE:
            return "CANDIDATE";
        case PSA_FWU_STAGED:
            return "STAGED";
        case PSA_FWU_FAILED:
            return "FAILED";
        case PSA_FWU_TRIAL:
            return "TRIAL";
        case PSA_FWU_REJECTED:
            return "REJECTED";
        case PSA_FWU_UPDATED:
            return "UPDATED";
        default:
            return "UNEXPECTED";
    }
}

static void update_state(psa_fwu_component_t component, uint8_t state)
{
  PRINTF("Component id:%d state updated\n%s", component,
            state_to_str(components_ctx[component].info.state));
    components_ctx[component].info.state = state;
    PRINTF(" -> %s\n", state_to_str(state));
}


/*******************************************************************************
 * Externs
 ******************************************************************************/

psa_status_t psa_fwu_query(psa_fwu_component_t component,
                           psa_fwu_component_info_t *info)
{
    fwu_ctx_t *p_component_ctx = &components_ctx[component];
    uint32_t boot_state;
    bl_image_info_t bl_image_info;
    
    if (info == NULL || component >= COMPONENTS_NUM) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }
    
    /* Init structs for first time run */
    if(p_component_ctx->initialized == false)
    {
        
        /* In debug session we assume active image to be version 0.0.0 for 
         * development purpose.
         */
        if(bl_get_active_image_info(component, &bl_image_info) != kStatus_Success)
        {
          PRINTF("Warning: Active image has no header (debug session?)\n");
          PRINTF("Image version: 0.0.0.0\n");
          bl_image_info.faid = 0;
          bl_image_info.header.ih_ver.iv_major = 0;
          bl_image_info.header.ih_ver.iv_minor = 0;
          bl_image_info.header.ih_ver.iv_revision = 0;
          bl_image_info.header.ih_ver.iv_build_num = 0;
        }
        p_component_ctx->info.version.major = bl_image_info.header.ih_ver.iv_major;
        p_component_ctx->info.version.minor = bl_image_info.header.ih_ver.iv_minor;
        p_component_ctx->info.version.patch = bl_image_info.header.ih_ver.iv_revision;
        p_component_ctx->info.version.build = bl_image_info.header.ih_ver.iv_build_num;
        p_component_ctx->info.max_size = IMAGE_SZ_MAX;
        p_component_ctx->info.flags = PSA_FWU_FLAG_VOLATILE_STAGING;
#ifdef CONFIG_ENCRYPT_XIP_EXT_ENABLE
      p_component_ctx->info.flags |= PSA_FWU_FLAG_ENCRYPTION;
#endif
        p_component_ctx->info.location = bl_image_info.faid;
        p_component_ctx->info.error = PSA_SUCCESS;
        
        if(bl_get_image_state(component, &boot_state) != kStatus_Success)
        {
          return PSA_ERROR_STORAGE_FAILURE;
        }
    
        /* The STAGED and TRIAL state can be clearly evaluated for all modes. The 
         * UPDATED state can be clearly stated only in DIRECT-XIP mode. Everything 
         * else after the reboot is considered as READY state as we assume that 
         * application ensures that the candidate slot is erased.
         */
        switch (boot_state)
        {
          case kSwapType_ReadyForTest:
            p_component_ctx->info.state = PSA_FWU_STAGED;
            break;
          case kSwapType_Testing:
            p_component_ctx->info.state = PSA_FWU_TRIAL;
            break;
          case kSwapType_Permanent:     //Note: Not clearly detected in SWAP
            p_component_ctx->info.state = PSA_FWU_UPDATED;
            break;
          default:
          p_component_ctx->info.state = PSA_FWU_READY;
        }
    
        components_ctx[component].initialized = true;
    }
    
    memcpy(info, &components_ctx[component].info, sizeof(psa_fwu_component_info_t));
    
    return PSA_SUCCESS;
}

psa_status_t psa_fwu_start(psa_fwu_component_t component,
                           const void *manifest,
                           size_t manifest_size)
{
    if (component >= COMPONENTS_NUM) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }
    if (components_ctx[component].initialized == false) {
        return PSA_ERROR_NOT_PERMITTED;
    }
    if (components_ctx[component].info.state != PSA_FWU_READY) {
        return PSA_ERROR_BAD_STATE;
    }
    
    partition_t update_prt;
    components_ctx[component].recv_sz = 0;
    
    if(bl_get_update_partition_info(component, &update_prt) != kStatus_Success)
        return PSA_ERROR_STORAGE_FAILURE;
    
    components_ctx[component].info.impl.update_phy_start = update_prt.start;
    components_ctx[component].info.impl.update_phy_size = update_prt.size;
    
    update_state(component, PSA_FWU_WRITING);
    
    return PSA_SUCCESS;  
}

psa_status_t psa_fwu_write(psa_fwu_component_t component,
                           size_t image_offset,
                           const void *block,
                           size_t block_size)
{
    uint32_t addr_phy = components_ctx[component].info.impl.update_phy_start;
    uint32_t max_size = components_ctx[component].info.impl.update_phy_size;
    size_t *recv_sz = &components_ctx[component].recv_sz;
  
    if (component >= COMPONENTS_NUM) 
        return PSA_ERROR_DOES_NOT_EXIST;
    if (components_ctx[component].initialized == false)
        return PSA_ERROR_NOT_PERMITTED;
    if (components_ctx[component].info.state != PSA_FWU_WRITING)
        return PSA_ERROR_BAD_STATE;
    if(*recv_sz + block_size > max_size || 
       block == NULL || block_size > PSA_FWU_MAX_WRITE_SIZE || block_size == 0)
        return PSA_ERROR_INVALID_ARGUMENT;
    
    
    
    if(flash_write(addr_phy + image_offset, block, block_size) != kStatus_Success)
    {
        PRINTF("Flash write failed %d");
        update_state(component, PSA_FWU_FAILED);
        return PSA_ERROR_STORAGE_FAILURE;
    }
    
    *recv_sz += block_size;
    
    return PSA_SUCCESS;  
}

psa_status_t psa_fwu_finish(psa_fwu_component_t component)
{
    if (component >= COMPONENTS_NUM) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }
    if (components_ctx[component].initialized == false) {
        return PSA_ERROR_NOT_PERMITTED;
    }
    if (components_ctx[component].info.state != PSA_FWU_WRITING)
        return PSA_ERROR_BAD_STATE;
    
    uint32_t addr_phy = components_ctx[component].info.impl.update_phy_start;
    
    if(bl_verify_image(addr_phy, components_ctx[component].recv_sz) == 0)
    {
        update_state(component, PSA_FWU_FAILED);
        return PSA_ERROR_DATA_INVALID;
    }
    
    update_state(component, PSA_FWU_CANDIDATE);
    
    return PSA_SUCCESS;  
}

psa_status_t psa_fwu_cancel(psa_fwu_component_t component)
{
    if (component >= COMPONENTS_NUM) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }
    if (components_ctx[component].initialized == false) {
        return PSA_ERROR_NOT_PERMITTED;
    }
    if (components_ctx[component].info.state == PSA_FWU_WRITING || 
        components_ctx[component].info.state == PSA_FWU_CANDIDATE)
    {
        update_state(component, PSA_FWU_FAILED);
        return PSA_SUCCESS;  
    }
    else
    {
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_fwu_clean(psa_fwu_component_t component)

{
    if (component >= COMPONENTS_NUM) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }
    if (components_ctx[component].initialized == false) {
        return PSA_ERROR_NOT_PERMITTED;
    }
    if (components_ctx[component].info.state == PSA_FWU_FAILED || 
        components_ctx[component].info.state == PSA_FWU_UPDATED)
    {
        PRINTF("Erasing update partition...\n");
        
        partition_t staging_prt;
        
        if(bl_get_update_partition_info(component, &staging_prt) != kStatus_Success)
        {
            return PSA_ERROR_BAD_STATE;
        }
        uint32_t slotaddr     = staging_prt.start;
        uint32_t slotsize     = staging_prt.size;
        uint32_t slotcnt      = (slotsize-1 + MFLASH_SECTOR_SIZE) / MFLASH_SECTOR_SIZE;

        for (int i = 0; i < slotcnt; i++)
        {
            if (mflash_drv_sector_erase(slotaddr) != kStatus_Success)
            {
                PRINTF("Failed to erase sector at 0x%X\n", slotaddr);
                return PSA_ERROR_STORAGE_FAILURE;
            }
            if (i % 10 == 0)
                PUTCHAR('.');
            slotaddr += MFLASH_SECTOR_SIZE;
        }
        PRINTF("\nDone\n");
        update_state(component, PSA_FWU_READY);
        return PSA_SUCCESS;  
    }
    else
    {
        return PSA_ERROR_BAD_STATE;
    }
}

psa_status_t psa_fwu_install(void)
{
    /* For sake of simplicity the integrity and signature check of the image is 
     * deferred to system reboot as this is untrusted staging model implementation.
     * The brief verification of update was done for candidate state.
     */
    int i;
    bool candidate_found = false;
    
    for (i = 0; i < COMPONENTS_NUM; i++)
    {
        fwu_ctx_t *p_comp = &components_ctx[i];
        
        if (p_comp->initialized == false)
            return PSA_ERROR_NOT_PERMITTED;
        if(p_comp->info.state == PSA_FWU_CANDIDATE)
        {
           bl_update_image_state(i, kSwapType_ReadyForTest);
           update_state(i, PSA_FWU_STAGED);
           candidate_found = true;
        }
    }
    
    if(candidate_found == false)
        return PSA_ERROR_BAD_STATE;
    
    return PSA_SUCCESS_REBOOT; 
}

psa_status_t psa_fwu_request_reboot(void)
{
    PRINTF("System reset!\n");
    NVIC_SystemReset();
    /* return PSA_SUCCESS;   */
}

psa_status_t psa_fwu_reject(psa_status_t error)
{
    int i;
    bool trial_found = false, staged_found = false;
    
    for (i = 0; i < COMPONENTS_NUM; i++)
    {
        fwu_ctx_t *p_comp = &components_ctx[i];
        
        if (p_comp->initialized == false)
            return PSA_ERROR_NOT_PERMITTED;
        if(p_comp->info.state == PSA_FWU_STAGED)
        {
           update_state(i, PSA_FWU_FAILED);
           p_comp->info.error = error;
           staged_found = true;
        }
        if(p_comp->info.state == PSA_FWU_TRIAL)
        {
           update_state(i, PSA_FWU_REJECTED);
           p_comp->info.error = error;
           trial_found = true;
        }
    }
    
    if(staged_found == false && trial_found == false)
        return PSA_ERROR_BAD_STATE;
    
    if(trial_found == true)
        return PSA_SUCCESS_REBOOT;
    
    return PSA_SUCCESS;  
}

psa_status_t psa_fwu_accept(void)
{
#ifdef MCUBOOT_OVERWRITE_ONLY
    return PSA_ERROR_NOT_SUPPORTED;
#else
    int i;
    bool trial_found = false;
    
    for (i = 0; i < COMPONENTS_NUM; i++)
    {
        fwu_ctx_t *p_comp = &components_ctx[i];
        
        if (p_comp->initialized == false)
            return PSA_ERROR_NOT_PERMITTED;
        if(p_comp->info.state == PSA_FWU_TRIAL)
        {
           bl_update_image_state(i, kSwapType_Permanent);
           update_state(i, PSA_FWU_UPDATED);
           trial_found = true;
        }
    }
    
    if(trial_found == false)
        return PSA_ERROR_BAD_STATE;
    
    return PSA_SUCCESS;
#endif
}
