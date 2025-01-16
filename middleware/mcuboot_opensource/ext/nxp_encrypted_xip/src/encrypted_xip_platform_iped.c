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
#if defined(ENCRYPTED_XIP_IPED) && defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE)
#include <ctype.h>
#include "encrypted_xip.h"

#include "fsl_debug_console.h"
#include "fsl_trng.h"
#include "fsl_cache.h"
#include "fsl_iped.h"
#include "fsl_romapi_iap.h"
#include "fsl_iped.h"

#include "flash_partitioning.h"
#include "mflash_drv.h"
#include "sysflash/sysflash.h"
#include "flash_map.h"
#include "mbedtls/md5.h"
  
#include "mcux_els.h"
#include "mcuxCsslParamIntegrity.h"
#include "mcuxCsslMemory.h"
#include "mcuxClEls_Aead.h"
#include "mcuxClEls_Rng.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * @brief Helper to assert function return status.
 *
 * @details Print finshed message and trap forever upon failure.
 */
#define ASSERT_IPED(expected, actual, ...) \
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

#if !defined(CONFIG_ENCRYPT_XIP_IPED_REGION_SIZE)
#error "Please define IPED region size what suits boundaries of target flash device"
#endif

/* Encryption metadata or mcuboot trailer are not encrypted, reserve flash sector */
#define IPED_REGION_SIZE     CONFIG_ENCRYPT_XIP_IPED_REGION_SIZE
#define IPED_REGION_EXEC_NUM 1

#define FLASH_FCB_OFFSET (BOOT_FLASH_BASE + 0x400) //FCB of main app

/********************************ROM code begin********************************/
#define MCUXCLELS_AEAD_IV_BLOCK_SIZE            16U             ///< AES-GCM IV Granularity:  128 bit (16 bytes)
#define MCUXCLELS_CIPHER_BLOCK_SIZE_AES         ((size_t) 16U)  ///< Size of an AES input block: 128 bit (16 bytes)

const uint8_t IPED_IV_ENCRYPT_AAD[16] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define NBOOT_IPED_REGION_COUNT                 (4)
#define NBOOT_IPED_REGION_CONFIG_ENABLED_MASK   (0x01)
#define NBOOT_IPED_REGION_CONFIG_LOCKED_MASK    (0x02)

#define FLEXSPI_IPED_CFG_BLK_TAG                (0x42435049UL) // ascii "IPCB" Big Endian
/*********************************ROM code end*********************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/********************************ROM code begin********************************/
typedef uint32_t nboot_status_t;
#define kStatus_NBOOT_Success         ((nboot_status_t) 0x5A5A5A5Au)    /*!< Operation completed successfully. */
#define kStatus_NBOOT_Fail            ((nboot_status_t) 0x5A5AA5A5u)    /*!< Operation failed. */
#define kStatus_NBOOT_InvalidArgument ((nboot_status_t) 0x5A5AA5F0u)    /*!< Invalid argument passed to the function. */

typedef enum
{
    kNBOOT_MemCryptIped = 0x95959595u,
} nboot_mem_crypt_engine_type_t;

typedef enum
{
    kNBOOT_MemCrypt12Rounds = 0x3c5ac33cu,
    kNBOOT_MemCrypt22Rounds = 0x5aa55aa5u,
} nboot_mem_crypt_prince_rounds_t;


typedef struct nboot_iped_encrypted_iv_t {
    uint8_t iv[MCUXCLELS_AEAD_IV_BLOCK_SIZE];
    uint8_t ciphertext[MCUXCLELS_CIPHER_BLOCK_SIZE_AES];
    uint8_t tag[MCUXCLELS_CIPHER_BLOCK_SIZE_AES];
} nboot_iped_encrypted_iv_t;

typedef struct nboot_iped_region_config
{
    uint32_t                        startAddress;   // IPEDx_START value
    uint32_t                        endAddress;     // IPEDx_END value
    uint8_t                         regionFlags;    // flags[0]: enabled,  flags[1]: locked
    uint8_t                         padding[3];
    nboot_iped_encrypted_iv_t       encryptedIv;
} nboot_iped_region_config_t;

typedef struct nboot_mem_crypt_encrypt_range_params {
    uint32_t                        startAddress;
    uint32_t                        endAddress;
    uint8_t                         regionFlags;     // flags[0]: enabled,  flags[1]: locked
} nboot_mem_crypt_encrypt_range_params_t;

typedef struct nboot_mem_crypt_configure_parms
{
    nboot_mem_crypt_engine_type_t configId;           // Used to determine whether PRINCE or IPED should be configured
    nboot_mem_crypt_prince_rounds_t ipedPrinceRounds; // should be set to kNBOOT_MemCrypt12Rounds or kNBOOT_MemCrypt22Rounds 
    iped_region_t regionOffset;                       // The index of the lowest IPED context register to use
    nboot_iped_region_config_t ipedConfig[NBOOT_IPED_REGION_COUNT];
} nboot_mem_crypt_configure_parms_t;

typedef struct _flexspi_nor_mem_image_iped_config
{
    uint32_t tag;
    nboot_mem_crypt_configure_parms_t params;
} flexspi_nor_mem_image_iped_config_t;
/*********************************ROM code end*********************************/


/*
 * Assume that metadata and IPED configuration structs can be written separately 
 * into common flash sector.
 */
_Static_assert(sizeof(flexspi_nor_mem_image_iped_config_t) <= MFLASH_SECTOR_SIZE/2,
		"Size of iped cfg exceeds flash page size");
/*******************************************************************************
 * Variables
 ******************************************************************************/
static api_core_context_t apiCoreCtx;
static uint8_t iap_api_arena[0x6000];

/*******************************************************************************
 * Static
 ******************************************************************************/
static void print_iped_cfgs(void)
{
    uint8_t iv[8] = {0};
    for (int i = 0; i < 4; i++) {
        uint32_t start_address = 0;
        uint32_t end_address = 0;
        IPED_GetRegionAddressRange(FLEXSPI, (iped_region_t) i, &start_address, &end_address);
        IPED_GetRegionIV(FLEXSPI, (iped_region_t) i, iv);
        PRINTF("region %d: start: 0x%x, address: 0x%x, iv %02x%02x%02x%02x%02x%02x%02x%02x\r\n", i, start_address, end_address, iv[0], iv[1] , iv[2] , iv[3] , iv[4] , iv[5] , iv[6] , iv[7]);
    }
}
/********************************ROM code begin********************************/
static nboot_status_t nboot_mem_crypt_decrypt_iv(const nboot_iped_encrypted_iv_t* encrypted_iv, uint8_t* plain_iv)
{
    nboot_status_t status = kStatus_NBOOT_Fail;

    do {
        mcuxClEls_AeadOption_t options;
        options.word.value = 0;
        options.bits.dcrpt = MCUXCLELS_AEAD_DECRYPT;
        options.bits.extkey = MCUXCLELS_AEAD_INTERN_KEY;
        uint8_t aead_ctx[MCUXCLELS_AEAD_CONTEXT_SIZE] = { 0 };
        
        /* The key that the boot rom uses for IV encryption is left in keyslot 3 by boot rom */
        mcuxClEls_KeyIndex_t key_index = (mcuxClEls_KeyIndex_t) 3;
        
        // iv
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result4, token, mcuxClEls_Aead_Init_Async(
                                           options,
                                           key_index,
                                           NULL,
                                           0U,
                                           encrypted_iv->iv,
                                           MCUXCLELS_AEAD_IV_BLOCK_SIZE,
                                           aead_ctx));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Aead_Init_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result4)) {
            PRINTF("mcuxClEls_Aead_Init_Async(...) failed: result: 0x%08x, token: 0x%08x\n", result4, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();

        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result5, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result5)) {
            PRINTF("mcuxClEls_WaitForOperation(...) failed: result: 0x%08x, token: 0x%08x\n", result5, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
        
        
        // aad
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result6, token, mcuxClEls_Aead_UpdateAad_Async(
                                           options,
                                           key_index,
                                           NULL,
                                           0U,
                                           IPED_IV_ENCRYPT_AAD,
                                           sizeof(IPED_IV_ENCRYPT_AAD),
                                           aead_ctx));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Aead_UpdateAad_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result6)) {
            PRINTF("mcuxClEls_Aead_UpdateAad_Async(...) failed: result: 0x%08x, token: 0x%08x\n", result6, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();

        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result7, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result7)) {
            PRINTF("mcuxClEls_WaitForOperation(...) failed: result: 0x%08x, token: 0x%08x\n", result7, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
        

        // data
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result8, token, mcuxClEls_Aead_UpdateData_Async(
                                           options,
                                           key_index,
                                           NULL,
                                           0U,
                                           encrypted_iv->ciphertext,
                                           MCUXCLELS_CIPHER_BLOCK_SIZE_AES,
                                           plain_iv,
                                           aead_ctx));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Aead_UpdateData_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result8)) {
            PRINTF("mcuxClEls_Aead_UpdateData_Async(...) failed: result: 0x%08x, token: 0x%08x\n", result8, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
        
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result9, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result9)) {
            PRINTF("mcuxClEls_WaitForOperation(...) failed: result: 0x%08x, token: 0x%08x\n", result9, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();


        // finalize
        uint8_t calculated_tag[MCUXCLELS_AEAD_IV_BLOCK_SIZE] = { 0 };
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result10, token, mcuxClEls_Aead_Finalize_Async(
                                            options,
                                            key_index,
                                            NULL,
                                            0U,
                                            sizeof(IPED_IV_ENCRYPT_AAD),
                                            MCUXCLELS_AEAD_IV_BLOCK_SIZE,
                                            calculated_tag,
                                            aead_ctx));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Aead_Finalize_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result10)) {
            PRINTF("mcuxClEls_Aead_Finalize_Async(...) failed: result: 0x%08x, token: 0x%08x\n", result10, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();

        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result11, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result11)) {
            PRINTF("mcuxClEls_WaitForOperation(...) failed: result: 0x%08x, token: 0x%08x\n", result11, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
        
        
        // compare auth tag
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result12, token, mcuxCsslMemory_Compare(
                                                mcuxCsslParamIntegrity_Protect(
                                                    3u,
                                                    calculated_tag,
                                                    encrypted_iv->tag,
                                                    MCUXCLELS_AEAD_IV_BLOCK_SIZE),
                                                calculated_tag,
                                                encrypted_iv->tag,
                                                MCUXCLELS_AEAD_IV_BLOCK_SIZE));
        
        if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Compare) != token) || (MCUXCSSLMEMORY_STATUS_EQUAL != result12)) {
            PRINTF("nxpCsslMemory_Compare(...) failed: result: 0x%08x, token: 0x%08x\n", result12, token);
            break;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
        
        status = kStatus_NBOOT_Success;
    } while(0);

    return status;
}


static nboot_status_t nboot_mem_crypt_load_iv(const iped_region_t region, const nboot_iped_encrypted_iv_t* encrypted_iv) 
{
    if (region >= IPED_REGION_COUNT)
    {
        return kStatus_NBOOT_InvalidArgument;
    }
    
    if (encrypted_iv == NULL) 
    {
        return kStatus_NBOOT_InvalidArgument;
    }
    
    nboot_status_t status = kStatus_NBOOT_Fail;
    /* The plain IV after decryption - only 8 bytes are used, but we need to fill the AES block size. */
    uint8_t plain_iv[MCUXCLELS_CIPHER_BLOCK_SIZE_AES] = {0}; 
    do {
        status = nboot_mem_crypt_decrypt_iv(encrypted_iv, plain_iv);
        if (status != kStatus_NBOOT_Success)
        {
            /* This normally does not happen. It means there is an IPED region configured, but 
             * either an IV never has been written to the IV store or the IV store somehow got 
             * corrupted. In order to avoid reusing an IV (either by accident or by an attack), 
             * we use a random IV in this error-case. The region is not readable, and writes 
             * will not reveal any information.
             */
            PRINTF("nboot_mem_crypt_decrypt_iv failed for region %d: 0x%08x, falling back to random IV\n", region, status);
            
            MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result0, token, mcuxClEls_Prng_GetRandom(plain_iv, sizeof(plain_iv)));
            if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Prng_GetRandom) != token) || (MCUXCLCSS_STATUS_OK != result0)) 
            {
                PRINTF("mcuxClEls_Prng_GetRandom(...) failed: result: 0x%08x, token: 0x%08x\n", result0, token);
                break;
            }
            MCUX_CSSL_FP_FUNCTION_CALL_END();
        }

        /* Load IV into IPED registers */
        IPED_SetRegionIV(FLEXSPI, region, plain_iv);
    } while(0);
    
    /* Todo Clear plain_iv from memory */
    /* Bootrom uses nboot_memset here which requires an nboot context. It overwrites with random 
     * data, and needs to keep track of the state of the RNG to determine when to do a reseeding. 
     * The IV is written in plain to SFRs from where it also can be read again, so this perhaps 
     * is excessive solution. 
     * Please use a "secure enough" memset for your use-case if needed. For simplicity we use
     * libc memset here. 
     */
    memset(plain_iv, 0u, sizeof(plain_iv));
    return status;
}


static nboot_status_t nboot_mem_crypt_configure(nboot_mem_crypt_configure_parms_t *parms)
{
    uint32_t region;
    //iped_prince_rounds_t ipedPrinceRounds = kIPED_PrinceRounds22;
    //nboot_bool_t isIpedEnabled = kNBOOT_FALSE; //If we boot from an IPED XIP sbl it is already enabled
    nboot_status_t status = kStatus_NBOOT_Fail;

    do
    {
        if (parms == NULL)
        {
            PRINTF("params is NULL");
            status = kStatus_NBOOT_InvalidArgument;
            break;
        }

        status = kStatus_NBOOT_Success;
        if (parms->configId == kNBOOT_MemCryptIped)
        {
            /* NOTE: bootrom already does that, it can not be re-configured, the
             * application MUST use the same number of rounds as the mcuboot 
             * as all regions share the same number of rounds.
             */
            /* Set IPED PRINCE rounds configuration */
            // ipedPrinceRounds = (parms->ipedPrinceRounds == kNBOOT_MemCrypt12Rounds) ? kIPED_PrinceRounds12 : kIPED_PrinceRounds22;
            // IPED_SetPrinceRounds(FLEXSPI, ipedPrinceRounds);

            /* Iterate for all IPED regions */
            for (size_t i = 0;  i < NBOOT_IPED_REGION_COUNT; i++)
            {
                /* Skip configuration of disabled regions */
                if ((parms->ipedConfig[i].regionFlags & NBOOT_IPED_REGION_CONFIG_ENABLED_MASK) != NBOOT_IPED_REGION_CONFIG_ENABLED_MASK)
                {
                    continue;
                }

                region = i + parms->regionOffset;

                ///* IPED encryption is enabled */
                //isIpedEnabled = kNBOOT_TRUE;

                /* Write start & end addresses to IPED registers */
                status = IPED_SetRegionAddressRange(FLEXSPI, region, parms->ipedConfig[i].startAddress, parms->ipedConfig[i].endAddress);
                if (status != kStatus_Success)
                {
                    PRINTF("IPED_SetRegionAddressRange failed for region %d: 0x%08x, startAddress: 0x%08x, endAddress: 0x%08x\n", i, 
                            status, parms->ipedConfig[i].startAddress, parms->ipedConfig[i].endAddress);
                    break;
                }
                status = IPED_SetRegionEnable(FLEXSPI, region, true);
                if (status != kStatus_Success)
                {
                    PRINTF("IPED_SetRegionEnable failed for region %d: 0x%08x\n", i, status);
                    break;
                }
                
                status = nboot_mem_crypt_load_iv(region, &parms->ipedConfig[i].encryptedIv);
                if (status != kStatus_NBOOT_Success)
                {
                    PRINTF("nboot_mem_crypt_load_iv failed for region %d: 0x%08x\n", i, status);
                    break;
                }

                /* Lock region if required */
                if ((parms->ipedConfig[i].regionFlags & NBOOT_IPED_REGION_CONFIG_LOCKED_MASK) == NBOOT_IPED_REGION_CONFIG_LOCKED_MASK)
                {
                    IPED_SetLock(FLEXSPI, region);
                }
            }

            /* Enable IPED encryption/decryption in case at least one region is enabled */
            //if (isIpedEnabled != kNBOOT_FALSE)
            //{
            //    IPED_EncryptEnable(FLEXSPI);
            //}
        }
        else
        {
            status = kStatus_NBOOT_InvalidArgument;
            break;
        }
    } while(0);
    
    return status;
}
/*********************************ROM code end*********************************/

/*******************************************************************************
 * Externs
 ******************************************************************************/
status_t platform_enc_init(void)
{
    status_t status = kStatus_Fail;
    const kp_api_init_param_t apiInitParam = 
          {.allocStart = (uint32_t) &iap_api_arena, 
           .allocSize = sizeof(iap_api_arena)
          };
    
    status = iap_api_init(&apiCoreCtx, &apiInitParam);
    if(status != kStatus_Success) {
        PRINTF("iap_api_init returned with code 0x%X\n", status);
        return status;
    }  
    status = iap_mem_config(&apiCoreCtx, (uint32_t *) FLASH_FCB_OFFSET, kMemoryID_FlexspiNor);
    if(status != kStatus_Success) {
        PRINTF("iap_mem_config returned with code 0x%X\n", status);
        return status;
    }
    return status;
}

size_t platform_enc_cfg_getSize(void)
{
    return sizeof(flexspi_nor_mem_image_iped_config_t);
}

status_t platform_enc_cfg_write(struct flash_area *fa_meta)
{
    uint32_t off_meta = 0;
    status_t status = kStatus_Fail;
    const uint32_t iped_region0_start = BOOT_FLASH_EXEC_APP;
    const uint32_t iped_region0_end = BOOT_FLASH_EXEC_APP + IPED_REGION_SIZE;
    
    flexspi_iped_config_arg_t iped_config = { 
        .option = { 
            .tag = FLEXSPI_IPED_CONFIG_TAG,
            .offset = IPED_REGION_EXEC_NUM,
            .count = 1, 
        },
        .prince_rounds = flexspi_iped_12_rounds, //must match what is used for other regions
        .regions = { 
            { .start = iped_region0_start, .end = iped_region0_end, .locked = 0 }, 
            { 0 }, 
            { 0 }, 
            { 0 }, 
        },
    };
    
    flexspi_iped_write_arg_t iped_write_arg = { 
      .option = {
          .tag = FLEXSPI_WRITE_IPED_CFG_BLK_FOR_IMAGE_TAG,
          .offset = 0,
          .count = 3, 
      },
      .address = fa_meta->fa_off + BOOT_FLASH_BASE,
    };
    
    /* use ROM API to configure IPED region, this also generates new IV */
    status = iap_mem_config(&apiCoreCtx, (uint32_t *)&iped_config, kMemoryID_FlexspiNor);
    if(status != kStatus_Success)
    {
        PRINTF("iap_mem_config returned with code 0x%X\n", status);
        return -1;
    }
       
    /* persist iped configuration at particular flash offset */
    status = iap_mem_erase(&apiCoreCtx, BOOT_FLASH_ENC_META, 0x1000, kMemoryID_FlexspiNor);
    if(status != kStatus_Success)
    {
        PRINTF("iap_mem_erase returned with code 0x%X\n", status);
        return -1;
    }
    status = iap_mem_config(&apiCoreCtx, (uint32_t *)&iped_write_arg, kMemoryID_FlexspiNor);
    if(status != kStatus_Success)
    {
        PRINTF("iap_mem_config returned with code 0x%X\n", status);
        return -1;
    }
    
    /* clear AHB RX buffer and cache */
    FLEXSPI->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK;
    CACHE64_InvalidateCache(CACHE64_CTRL0);
        
    return kStatus_Success;
}

status_t platform_enc_cfg_initEncryption(struct flash_area *fa_meta)
{
    nboot_status_t status = kStatus_NBOOT_Fail;
    bool is_iped_region_enabled = false;
    
    uint32_t address = fa_meta->fa_off + BOOT_FLASH_BASE;
    flexspi_nor_mem_image_iped_config_t* config = (flexspi_nor_mem_image_iped_config_t*) address;
    if (config->tag != FLEXSPI_IPED_CFG_BLK_TAG) {
        PRINTF("No IPED configuration found!\n");
        goto error;
    }

    for (size_t i = 0; i < ARRAY_SIZE(config->params.ipedConfig); i++) {
        is_iped_region_enabled = is_iped_region_enabled \
            || ((config->params.ipedConfig[i].regionFlags & NBOOT_IPED_REGION_CONFIG_ENABLED_MASK) == NBOOT_IPED_REGION_CONFIG_ENABLED_MASK);
    }
    if (! is_iped_region_enabled) {
        PRINTF("No IPED region enabled!\n");
        goto error;
    }

    /*  Copy the parameters from Flash to RAM (perhaps not necessary, but since 
     *  are reconfiguring FLEXSPI settings in the following, it may be safer, not
     *  to access Flash at the same time).
     */
    nboot_mem_crypt_configure_parms_t mem_crypt_cfg = config->params;

    status = nboot_mem_crypt_configure(&mem_crypt_cfg);
    if (status != kStatus_NBOOT_Success)
    {
        PRINTF("nboot_mem_crypt_configure failed: 0x%08x\n", status);
        goto error;
    }
    PRINTF("IPED configuration found and successfully configured...\n");
    
    IPED_EncryptEnable(FLEXSPI);
    
    FLEXSPI->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK;
    CACHE64_InvalidateCache(CACHE64_CTRL0);
    
    return kStatus_Success;
error:
    PRINTF("Skipping IPED configuration!\n");
    return kStatus_Fail;
}

/* In case of PRINCE based encryption units there is a risk when accessing to
 * unwritten pages which leads to crash of PRINCE module.
 * This operation ensures the integrity of IPED configuration of the image in
 * execution area.
 */
bool platform_enc_cfg_isPresent(uint32_t addr)
{
    flexspi_nor_mem_image_iped_config_t *iped_config = (flexspi_nor_mem_image_iped_config_t*) addr;
    if (iped_config->tag == FLEXSPI_IPED_CFG_BLK_TAG) {
        return true;
    }
    PRINTF("No IPED configuration found!\n");
    return false;
}

status_t platform_enc_cfg_getNonce(struct flash_area *fa_meta, uint8_t *nonce)
{
    /* Nonce is not needed in this case as IPED encryption is done on-the-fly */
    return kStatus_Success;
}

status_t platform_enc_finish(void)
{
    iap_api_deinit(&apiCoreCtx);
    return kStatus_Success;
}

status_t platform_enc_encrypt_data(uint32_t flash_addr, uint8_t *nonce, 
                                   uint8_t *input, uint8_t *output, uint32_t len)
{
    /* Nothing needed here as encryption is done on-the-fly */
    return kStatus_Success;
}

status_t platform_enc_flash_write(const struct flash_area *area, uint32_t off, const void *src, uint32_t len)
{
    uint32_t addr_off = area->fa_off + off + BOOT_FLASH_BASE;
    status_t rc;
    /* IPED requires that size of flash write to IPED region is always a multiple of 4 pages in size */
    ASSERT_IPED(len, 4*MFLASH_PAGE_SIZE, "Flash write size requires 0x%X bytes not 0x%X\n", 4*MFLASH_PAGE_SIZE, len);
    rc = iap_mem_write_blocked(&apiCoreCtx, addr_off, len, (uint8_t*) src, kMemoryID_FlexspiNor);
    if(rc != kStatus_Success){
        PRINTF("\niap_mem_write_blocked failed with code %d, addr_off 0x%X len 0x%X\n", rc, addr_off, len);
    }
    return rc;
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

#endif /* ENCRYPTED_XIP_IPED */
