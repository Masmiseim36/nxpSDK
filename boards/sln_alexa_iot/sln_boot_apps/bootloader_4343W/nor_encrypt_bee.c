/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "bl_drv/fsl_dcp_ext.h"
#include "fsl_trng.h"
#include "nor_encrypt_bee.h"
#include "fusemap.h"
#include "fica_definition.h"
#include "sln_auth.h"
#include "sln_flash.h"

#if defined(FSL_RTOS_FREE_RTOS)
#include "FreeRTOS.h"
#include "task.h"

/* AWS includes */
#include AWS_LOGGING_INCLUDE

#define DEBUG_LOG_DELAY_MS(x) vTaskDelay(portTICK_PERIOD_MS *x)
#define DEBUG_LOG(x) configPRINTF(x)
#else
#define DEBUG_LOG_DELAY_MS(x)
#define DEBUG_LOG(X)
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define KIB_ADDR(index) (FlexSPI_AMBA_BASE + 0x400 * (index + 1))
#define PRDB_ADDR(index) (FlexSPI_AMBA_BASE + 0x400 * (index + 1) + 0x80)
#define MIN_ENC_REGION_ADDR (FlexSPI_AMBA_BASE + 0x1000)

const uint32_t kFacRegionCount = 1U;

enum
{
    kAesMode_ECB = 0, // AES ECB mode is required
    kAesMode_CTR = 1, // AES CTR mode is required.
};

enum
{
    kBeeKeySel_OTPMK_SNVS_Low  = 1, //!< BEE Key from OTPMK/SNVS[127:0]
    kBeeKeySel_OTPMK_SNVS_High = 2, //!< BEE Key from OTPMK/SNVS[255:128]
    kBeeKeySel_SW_GP2          = 3, //!< BEE Key from SW_GP2[127:0]
    kBeeKeySel_User_Key        = 4, //!< Key was provided by user
};

enum
{
    kBeeLockOption_NoLock      = 0, //!< BEE Region related bits are unlocked.
    kBeeLockOption_Region1     = 1, //!< BEE Region1 related bits are locked.
    kBeeLockOption_Region0     = 2, //!< BEE Region0 related bits are locked.
    kBeeLockOption_BothRegions = 3, //!< BEE regions related bits are locked.
};

enum
{
    kFacRegionLockOption_NoLock = 0, //!< FAC Region related bits are unlocked.
    kFacRegionLockOption_Lock   = 1, //!< FAC region related bits are locked.
};

typedef struct
{
    uint32_t start;       //!< Start address of the encrypted region, align at 1KB boundary
    uint32_t end;         //!< End address of the encrypted region, align at 1KB boundary
    uint32_t mode;        //!< AES mode: 0-CTR, 1-ECB
    uint32_t lock_option; //!< Lock options
    uint32_t counter[4];  //!< Counter for AES-CTR mode
    uint32_t reserved[8]; //!< Reserved for future use.
} encrypt_region_t;

//!@brief BEE Protection Region Descriptor Block related definitions
#define BEE_PROT_REGION_BLK_TAGL 0x5F474154 //"TAG_"
#define BEE_PROT_REGION_BLK_TAGH 0x52444845 //"EHDR"
#define BEE_PROT_REGION_HDR_VER 0x56010000  // Version 1.0.0

enum
{
    kFacMode_M7DebugAllowed          = 0,
    kFacMode_M7DebugDisabled         = 1,
    kFacMode_ExecuteOnlyDebugAllowed = 2,
    kFacMode_ExecuteOnly             = 3,
};

typedef struct
{
    uint32_t start;       //!< Start address of one FAC region, align at 1KB boundary
    uint32_t end;         //!< End address of one FAC region, align at 1KB boundary
    uint32_t mode;        //!< Protected level: 0/1/2/3
    uint32_t reserved[5]; //!< Reserved for future use
} fac_region_t;

//!@brief Maximum supported BEE protection entries
#define MAX_BEE_PROT_ENTRIES 3

typedef struct
{
    uint32_t tagl;                   //!< Lower Half of tag, equal to BEE_PROT_REGION_BLK_TAGL
    uint32_t tagh;                   //!< Upper Half of tag, equal to BEE_PROT_REGION_BLK_TAGH
    uint32_t version;                //!< Version
    uint32_t fac_region_count;       //!< FAC region count, valid value: 1-4
    encrypt_region_t encrypt_region; //!< Encrypted region info
    fac_region_t fac_regions[4];     //!< FAC region info
    uint32_t reserved1[12];          //!< Reserved for future use.
} prot_region_desc_block_t;

#define PROT_REGION_ALIGN_SIZE (0x1000)
#define FAC_REGION_ALIGN_SIZE (0x400)

typedef struct
{
    uint8_t aes_key[16]; // AES KEY
    uint8_t iv[16];      // AES Initial Vector
} key_info_block_t;

typedef struct
{
    key_info_block_t kib;          // KIB info
    uint8_t unused[0x60];          // Padding data
    prot_region_desc_block_t prdb; // PRDB info
} prot_region_block_info_t;

typedef struct
{
    uint32_t lock_option : 4;      // Lock options, valid range: 0/1/2/3
    uint32_t region2_fac_mode : 4; // Region 2 Fac mode, valid range: 0/1/2/3
    uint32_t region1_fac_mode : 4; // Region 1 Fac mode, valid range: 0/1/2/3
    uint32_t region0_fac_mode : 4; // Region 0 Fac mode, valid range: 0/1/2/3
    uint32_t fac_region_count : 4; // Fac region count in selected BEE region, valid range: 1/2/3
    uint32_t aes_mode : 4;         // Fac region count in selected BEE region, valid range: 1/2/3
    uint32_t key_source : 4;       // Key source: valid range 0/1/2, 4/5/6
    uint32_t tag : 4;              // Fixed to 0x0E
} prot_region_option_t;

enum
{
    kOption_KeySel_Begion          = 0,
    kOption_KeySel_OTPMK_SNVS_High = kOption_KeySel_Begion,
    kOption_KeySel_OTPMK_SNVS_Low,
    kOption_KeySel_SW_GP2,
    kOption_KeySel_End = kOption_KeySel_SW_GP2,
};

#define OPTION_BEE_REGION_KEY_SOURCE(idx, keysource) (((idx) << 2) | (keysource))
#define OPTION_BEE_REGION_KEY_SOURCE_MASK (0x3)

typedef struct
{
    uint32_t start;
    uint32_t end;
} bee_fac_region_t;

typedef struct
{
    prot_region_option_t option;
    struct
    {
        uint32_t start;
        uint32_t length;
    } fac_region[MAX_BEE_PROT_ENTRIES];
} prot_region_arg_t;

enum
{
    kProtRegionArg_Option_Tag = 0x0E,
};

typedef struct
{
    uint32_t fuse_key_sel[kHabMaxConfigCount];
    aes_key_sel_t bee_key_sel[kHabMaxConfigCount];
    bool encrypt_enabled[kHabMaxConfigCount];
    prot_region_block_info_t plain_block_info[kHabMaxConfigCount];
    prot_region_block_info_t enc_block_info[kHabMaxConfigCount];
} image_gen_ctx_t;

/*******************************************************************************
 * Local variables
 ******************************************************************************/
static image_gen_ctx_t s_img_gen_ctx;

/* RAM ctx backup of the in-place settings after a new context generation */
static image_gen_ctx_t s_img_gen_ctx_previous;

/* Contexts loaded from BACKUP region in RAM */
static image_gen_ctx_t s_img_gen_ctx_backup;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
// Swap bytes within a block
static void aes_block_swap(uint8_t array[16]);

// Try to load and parse Protection region info
static status_t bl_nor_encrypt_region_info_load_default(void);

// Check whether the option is valid or not
static bool is_valid_options(prot_region_block_info_t *option);

/*******************************************************************************
 * Codes
 ******************************************************************************/
static void loop_delay(void)
{
    volatile uint32_t delay = 1000000;
    while (delay)
    {
        delay--;
    }
}

static uint32_t bl_nor_encrypt_get_region_start(uint32_t index)
{
    uint32_t ret = FlexSPI_AMBA_BASE;

    if (kHabBootloader == index)
    {
        ret += FICA_IMG_BOOTLOADER_ADDR;
    }
    else if (kHabApplication == index)
    {
        ret += FICA_IMG_APP_A_ADDR;
    }
    else
    {
        ret = 0xFFFFFFFFU; // Invlaid address should indicate invalid input
    }

    return ret;
}

static uint32_t bl_nor_encrypt_get_region_end(uint32_t index)
{
    uint32_t ret = FlexSPI_AMBA_BASE;

    if (kHabBootloader == index)
    {
        ret += FICA_IMG_BOOTLOADER_ADDR + FICA_IMG_BOOTLOADER_SIZE + FICA_IMG_RESERVED_SIZE;
    }
    else if (kHabApplication == index)
    {
        ret += FICA_IMG_APP_A_ADDR + FICA_IMG_APP_A_SIZE + FICA_IMG_APP_B_SIZE;
    }
    else
    {
        ret = 0xFFFFFFFFU; // Invlaid address should indicate invalid input
    }

    return ret;
}

static status_t bl_nor_encrypt_region_info_load_from_offset(uint32_t offset, image_gen_ctx_t *img_gen_ctx)
{
    /*
     * NOTE: This function supports load EKIB that is encrypted by the key derived from OTPMK only.
     */
    status_t status   = kStatus_Fail;
    bool isEncryptXIP = ROM_OCOTP_ENCRYPT_XIP_VALAUE();
    bool isKey0SelSet = (ROM_OCOTP_BEE_KEY0_SEL_VALUE() > 0);
    bool isKey1SelSet = (ROM_OCOTP_BEE_KEY1_SEL_VALUE() > 0);

    uint32_t irqState;

    irqState = DisableGlobalIRQ();

    SCB_DisableDCache();

    img_gen_ctx->fuse_key_sel[0] = kBeeKeySel_OTPMK_SNVS_High;
    img_gen_ctx->fuse_key_sel[1] = kBeeKeySel_OTPMK_SNVS_High;

    do
    {
        /* Flashloader will not load the PRDB if one of below conditions is met */
        if (!isEncryptXIP)
        {
            break;
        }

        if (!isKey0SelSet && !isKey1SelSet)
        {
            break;
        }

        img_gen_ctx->fuse_key_sel[0] = ROM_OCOTP_BEE_KEY0_SEL_VALUE();
        img_gen_ctx->fuse_key_sel[1] = ROM_OCOTP_BEE_KEY1_SEL_VALUE();

        dcp_alg_ctx_t dcp_ctx;
        dcp_aes_init(&dcp_ctx);

        for (uint32_t i = 0; i < kHabMaxConfigCount; i++)
        {
            switch (img_gen_ctx->fuse_key_sel[i])
            {
                case kBeeKeySel_OTPMK_SNVS_Low:
                    img_gen_ctx->bee_key_sel[i].option = OTPMK_SNVS_LOW_FLAG_BE;
                    break;
                case kBeeKeySel_OTPMK_SNVS_High:
                    img_gen_ctx->bee_key_sel[i].option = OTPMK_SNVS_HIGH_FLAG_BE;
                    break;
                case kBeeKeySel_SW_GP2:
                    img_gen_ctx->bee_key_sel[i].option = SW_GP2_FLAG_BE;
                    break;
                default:
                    img_gen_ctx->bee_key_sel[i].key = NULL;
                    break;
            }

            if (img_gen_ctx->bee_key_sel[i].key == NULL)
            {
                img_gen_ctx->encrypt_enabled[i] = false;
                continue;
            }
            else
            {
                img_gen_ctx->encrypt_enabled[i] = true;
            }

            prot_region_block_info_t plain_block_info = {0};
            prot_region_block_info_t enc_block_info = {0};
            dcp_aes_set_key(&dcp_ctx, img_gen_ctx->bee_key_sel[i], 128);
            uint32_t kib_addr  = offset + KIB_ADDR(i);
            uint32_t prdb_addr = offset + PRDB_ADDR(i);

            // Read EKIB from Flash
            memcpy(&enc_block_info.kib, (void *)kib_addr, sizeof(key_info_block_t));
            // Read EPRDB from flash
            memcpy(&enc_block_info.prdb, (void *)prdb_addr, sizeof(prot_region_desc_block_t));

            // Decrypt EKIB
            dcp_aes_ecb_crypt(&dcp_ctx, kAesMode_Decrypt, (uint8_t *)&enc_block_info.kib,
                              (uint8_t *)&plain_block_info.kib, sizeof(key_info_block_t));

            // Decrypt PRDB
            aes_key_sel_t key_sel;
            key_sel.key = plain_block_info.kib.aes_key;
            dcp_aes_set_key(&dcp_ctx, key_sel, 128);
            dcp_aes_cbc_crypt(&dcp_ctx,                          // DCP context
                              kAesMode_Decrypt,                  // Decrypt
                              plain_block_info.kib.iv,           // Intial vector
                              (uint8_t *)&enc_block_info.prdb,   // Source, encrypted data
                              (uint8_t *)&plain_block_info.prdb, // Destination, decrypted data
                              sizeof(prot_region_desc_block_t)); // Size in bytes

            if ((plain_block_info.prdb.tagh != BEE_PROT_REGION_BLK_TAGH) ||
                (plain_block_info.prdb.tagl != BEE_PROT_REGION_BLK_TAGL))
            {
                img_gen_ctx->encrypt_enabled[i] = false;
                memset(&img_gen_ctx->plain_block_info[i], 0, sizeof(prot_region_block_info_t));
                continue;
            }
            else
            {
                img_gen_ctx->encrypt_enabled[i] = true;
                memcpy(&img_gen_ctx->plain_block_info[i], &plain_block_info, sizeof(prot_region_block_info_t));
                status = kStatus_Success;
            }
        }

    } while (0);

    SCB_EnableDCache();

    EnableGlobalIRQ(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ISB();

    return status;
}

static status_t bl_nor_encrypt_region_info_load_default(void)
{
    return bl_nor_encrypt_region_info_load_from_offset(0x0, &s_img_gen_ctx);
}

status_t bl_nor_encrypt_region_info_load_backup(void)
{
    return bl_nor_encrypt_region_info_load_from_offset(BACKUP_REGION, &s_img_gen_ctx_backup);
}

bool bl_nor_encrypt_has_encrypted_region(void)
{
    bool has_encrypted_region = false;
    status_t status           = bl_nor_encrypt_region_info_load_default();
    if (status == kStatus_Success)
    {
        has_encrypted_region = true;
    }

    return has_encrypted_region;
}

static void setup_key(uint32_t keySel)
{
    switch (keySel)
    {
        case kBeeKeySel_OTPMK_SNVS_Low:
            IOMUXC_GPR->GPR10 &= (uint32_t)~IOMUXC_GPR_DCP_KEY_MUX_SEL_MASK;
            IOMUXC_GPR->GPR3 &= (uint32_t)~IOMUXC_GPR_DCP_KEY_SEL_MASK;
            break;
        case kBeeKeySel_OTPMK_SNVS_High:
            IOMUXC_GPR->GPR10 &= (uint32_t)~IOMUXC_GPR_DCP_KEY_MUX_SEL_MASK;
            IOMUXC_GPR->GPR3 |= (uint32_t)IOMUXC_GPR_DCP_KEY_SEL_MASK;
            break;
        case kBeeKeySel_SW_GP2:
            IOMUXC_GPR->GPR10 |= IOMUXC_GPR_DCP_KEY_MUX_SEL_MASK;
            break;
        default:
            // Do nothing.
            break;
    }
}

void bl_nor_encrypt_set_key(void)
{
    bool isEncryptXIP   = ROM_OCOTP_ENCRYPT_XIP_VALAUE();
    uint32_t key0SelSet = ROM_OCOTP_BEE_KEY0_SEL_VALUE();
    uint32_t key1SelSet = ROM_OCOTP_BEE_KEY1_SEL_VALUE();

    if (isEncryptXIP)
    {
        setup_key(key0SelSet);
        setup_key(key1SelSet);
    }
}

bool bl_nor_encrypt_is_enabled(void)
{
    bool ret          = false;
    bool isEncryptXIP = ROM_OCOTP_ENCRYPT_XIP_VALAUE();
    bool isKey0SelSet = (ROM_OCOTP_BEE_KEY0_SEL_VALUE() > 0);
    bool isKey1SelSet = (ROM_OCOTP_BEE_KEY1_SEL_VALUE() > 0);

    ret = isEncryptXIP && (isKey0SelSet || isKey1SelSet);

    return ret;
}

bool bl_nor_encrypt_region_info_valid(void *arg)
{
    bool is_valid = false;

    do
    {
        prot_region_arg_t *region_arg = (prot_region_arg_t *)arg;
        if ((arg == NULL) || (region_arg->option.tag != kProtRegionArg_Option_Tag))
        {
            break;
        }

        is_valid = true;

    } while (0);

    return is_valid;
}

bool is_valid_options(prot_region_block_info_t *option)
{
    bool is_valid = false;
    do
    {
        uint32_t fac_region_count = option->prdb.fac_region_count;

        if ((fac_region_count < 1) || (fac_region_count > MAX_BEE_PROT_ENTRIES))
        {
            break;
        }

        if (kAesMode_CTR != option->prdb.encrypt_region.mode)
        {
            break;
        }

        // TODO: Extend to check all FAC regions
        if (option->prdb.fac_regions[kFacRegionCount - 1].mode > kFacMode_ExecuteOnly)
        {
            break;
        }

        is_valid = true;

    } while (0);

    return is_valid;
}

status_t bl_nor_encrypt_make_new_config(uint32_t index)
{
    status_t status = kStatus_InvalidArgument;
    trng_config_t trng_config;
    prot_region_block_info_t *block_info = NULL;

    if (kHabMaxConfigCount > index)
    {
        status = kStatus_Success;
    }

    if (kStatus_Success == status)
    {
        block_info = &s_img_gen_ctx.plain_block_info[index];

        /* backup current ctx */
        memcpy(&s_img_gen_ctx_previous, &s_img_gen_ctx, sizeof(image_gen_ctx_t));

        if (NULL != block_info)
        {
            // Clear out current config
            memset(block_info, 0, sizeof(prot_region_block_info_t));

            // Set Valid Tags
            block_info->prdb.tagh = BEE_PROT_REGION_BLK_TAGH;
            block_info->prdb.tagl = BEE_PROT_REGION_BLK_TAGL;

            // Set version
            block_info->prdb.version = BEE_PROT_REGION_HDR_VER;

            // Set number of FAC regions
            block_info->prdb.fac_region_count = kFacRegionCount;

            // Set memory region bounds
            block_info->prdb.encrypt_region.start = bl_nor_encrypt_get_region_start(index);
            block_info->prdb.encrypt_region.end   = bl_nor_encrypt_get_region_end(index);

            // Set FAC region
            for (uint32_t facIdx = 0; facIdx < block_info->prdb.fac_region_count; facIdx++)
            {
                block_info->prdb.fac_regions[facIdx].start = block_info->prdb.encrypt_region.start;
                block_info->prdb.fac_regions[facIdx].end   = block_info->prdb.encrypt_region.end;
                block_info->prdb.fac_regions[facIdx].mode  = 0U;
            }

            // set AES CTR mode
            block_info->prdb.encrypt_region.mode = kAesMode_CTR;
        }
        else
        {
            status = kStatus_OutOfRange;
        }
    }

    if (kStatus_Success == status)
    {
        // Initialize TRNG module before generating nonce and KIB
        status = TRNG_GetDefaultConfig(&trng_config);
    }

    if (kStatus_Success == status)
    {
        // Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.
        trng_config.sampleMode = kTRNG_SampleModeVonNeumann;
        status                 = TRNG_Init(TRNG, &trng_config);
    }

    if (kStatus_Success == status)
    {
        // Automatically generate random counter[127:32]
        status = TRNG_GetRandomData(TRNG, &block_info->prdb.encrypt_region.counter[0], 16);
        block_info->prdb.encrypt_region.counter[0] = 0;
        uint8_t *temp                              = (uint8_t *)(block_info->prdb.encrypt_region.counter);
        for (uint32_t idx = 0; idx < 16; idx++)
        {
            configPRINT_STRING("%02X", temp[idx]);
        }
        configPRINT_STRING("\r\n");
    }

    if (kStatus_Success == status)
    {
        // Generate random KIB info: aes_key
        status = TRNG_GetRandomData(TRNG, &block_info->kib.aes_key, 16);
    }

    if (kStatus_Success == status)
    {
        // Generate random KIB info: initial vector
        status = TRNG_GetRandomData(TRNG, &block_info->kib.iv, 16);
    }

    return status;
}

bool bl_nor_in_encrypted_region(uint32_t start, uint32_t bytes)
{
    bool is_in_encypted_region = false;
    for (uint32_t bee_region_index = 0; bee_region_index < 2; bee_region_index++)
    {
        prot_region_block_info_t *block_info = &s_img_gen_ctx.plain_block_info[bee_region_index];
        for (uint32_t i = 0; i < block_info->prdb.fac_region_count; i++)
        {
            if ((start > (block_info->prdb.fac_regions[i].end - 1)) ||
                ((start + bytes) <= block_info->prdb.fac_regions[i].start))
            {
                continue;
            }
            else
            {
                is_in_encypted_region = true;
                break;
            }
        }

        if (is_in_encypted_region)
        {
            break;
        }
    }

    return is_in_encypted_region;
}

status_t bl_nor_encrypt_get_config_block(uint32_t index, uint32_t **start, uint32_t *bytes)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((start == NULL) || (bytes == NULL) || (index > 1) || (!s_img_gen_ctx.encrypt_enabled[index]))
        {
            break;
        }

        dcp_alg_ctx_t dcp_ctx;
        dcp_aes_init(&dcp_ctx);
        aes_key_sel_t key_sel;

        // Encrypt PRDB
        prot_region_block_info_t *plain_block_info = &s_img_gen_ctx.plain_block_info[index];
        prot_region_block_info_t *enc_block_info   = &s_img_gen_ctx.enc_block_info[index];
        key_sel.key                                = &plain_block_info->kib.aes_key[0];
        dcp_aes_set_key(&dcp_ctx, key_sel, 128);
        dcp_aes_cbc_crypt(&dcp_ctx, kAesMode_Encrypt, &plain_block_info->kib.iv[0], (uint8_t *)&plain_block_info->prdb,
                          (uint8_t *)&enc_block_info->prdb, sizeof(prot_region_desc_block_t));

        // Enecrypt KIB
        key_sel.option = s_img_gen_ctx.bee_key_sel[index].option;
        dcp_aes_set_key(&dcp_ctx, key_sel, 128);
        dcp_aes_ecb_crypt(&dcp_ctx, kAesMode_Encrypt, (uint8_t *)&plain_block_info->kib,
                          (uint8_t *)&enc_block_info->kib, sizeof(key_info_block_t));

        *start = (uint32_t *)enc_block_info;
        *bytes = sizeof(*enc_block_info);

        status = kStatus_Success;

    } while (0);

    return status;
}

void aes_block_swap(uint8_t array[16])
{
    uint32_t i;
    uint8_t tmp;

    for (i = 0; i < 8; i++)
    {
        tmp           = array[i];
        array[i]      = array[15 - i];
        array[15 - i] = tmp;
    }
}

static status_t bl_nor_encrypt_data_with_img_ctx(uint32_t addr,
                                                 uint32_t size,
                                                 uint32_t *data_start,
                                                 image_gen_ctx_t *img_gen_ctx)
{
    status_t status = kStatus_InvalidArgument;
    dcp_alg_ctx_t dcp_ctx;
    aes_key_sel_t key_sel;

    do
    {
        if ((size == 0) || (data_start == NULL) || (NULL == img_gen_ctx))
        {
            break;
        }

        uint32_t bee_region_index = 0;

        while (bee_region_index < 2)
        {
            prot_region_block_info_t *plain_block_info = &img_gen_ctx->plain_block_info[bee_region_index];

            if ((addr > (plain_block_info->prdb.encrypt_region.end - 1)) ||
                ((addr + size) <= plain_block_info->prdb.encrypt_region.start))
            {
                bee_region_index++;
            }
            else
            {
                break;
            }
        }
        if (bee_region_index > 1)
        {
            break;
        }
        key_sel.option = img_gen_ctx->bee_key_sel[bee_region_index].option;

        dcp_aes_init(&dcp_ctx);

        dcp_aes_set_key(&dcp_ctx, key_sel, 128);

        prot_region_block_info_t *plain_block_info = &img_gen_ctx->plain_block_info[bee_region_index];
        if (kAesMode_CTR == plain_block_info->prdb.encrypt_region.mode)
        {
            uint32_t counter[4];
            memcpy(&counter, plain_block_info->prdb.encrypt_region.counter, 16);
            counter[0] = addr >> 4;
            // Convert to 128bit big-endian mode before doing AES-CTR encryption
            aes_block_swap((uint8_t *)&counter);
            // Do In-place encryption
            dcp_aes_ctr_crypt(&dcp_ctx, (uint8_t *)&counter, (uint8_t *)data_start, (uint8_t *)data_start, size);
        }
        else
        {
            // Do In-place encryption
            dcp_aes_ecb_crypt(&dcp_ctx, kAesMode_Encrypt, (uint8_t *)data_start, (uint8_t *)data_start, size);
        }

        status = kStatus_Success;

    } while (0);

    return status;
}

static status_t bl_nor_decrypt_data_with_img_ctx(uint32_t addr,
                                                 uint32_t size,
                                                 uint32_t *data_start,
                                                 image_gen_ctx_t *img_gen_ctx)
{
    status_t status = kStatus_InvalidArgument;
    dcp_alg_ctx_t dcp_ctx;
    aes_key_sel_t key_sel;

    do
    {
        if ((size == 0) || (data_start == NULL) || (NULL == img_gen_ctx))
        {
            break;
        }

        uint32_t bee_region_index = 0;

        while (bee_region_index < 2)
        {
            prot_region_block_info_t *plain_block_info = &img_gen_ctx->plain_block_info[bee_region_index];

            if ((addr > (plain_block_info->prdb.encrypt_region.end - 1)) ||
                ((addr + size) <= plain_block_info->prdb.encrypt_region.start))
            {
                bee_region_index++;
            }
            else
            {
                break;
            }
        }
        if (bee_region_index > 1)
        {
            break;
        }
        key_sel.option = img_gen_ctx->bee_key_sel[bee_region_index].option;

        dcp_aes_init(&dcp_ctx);

        dcp_aes_set_key(&dcp_ctx, key_sel, 128);

        prot_region_block_info_t *plain_block_info = &img_gen_ctx->plain_block_info[bee_region_index];
#if 0
        uint8_t *temp                              = (uint8_t *)(plain_block_info->prdb.encrypt_region.counter);
        for (uint32_t idx = 0; idx < 16; idx++)
        {
            configPRINT_STRING("%02X", temp[idx]);
        }
        configPRINT_STRING("\r\n");
#endif
        if (kAesMode_CTR == plain_block_info->prdb.encrypt_region.mode)
        {
            uint32_t counter[4];
            memcpy(&counter, plain_block_info->prdb.encrypt_region.counter, 16);
            counter[0] = addr >> 4;
            // Convert to 128bit big-endian mode before doing AES-CTR encryption
            aes_block_swap((uint8_t *)&counter);
            // Do In-place encryption
            dcp_aes_ctr_crypt(&dcp_ctx, (uint8_t *)&counter, (uint8_t *)data_start, (uint8_t *)data_start, size);
        }
        else
        {
            // Do In-place encryption
            dcp_aes_ecb_crypt(&dcp_ctx, kAesMode_Decrypt, (uint8_t *)data_start, (uint8_t *)data_start, size);
        }

        status = kStatus_Success;

    } while (0);

    return status;
}

status_t bl_nor_encrypt_data(uint32_t addr, uint32_t size, uint32_t *data_start)
{
    return bl_nor_encrypt_data_with_img_ctx(addr, size, data_start, &s_img_gen_ctx);
}

status_t bl_nor_encrypt_data_prev_ctx(uint32_t addr, uint32_t size, uint32_t *data_start)
{
    return bl_nor_encrypt_data_with_img_ctx(addr, size, data_start, &s_img_gen_ctx_previous);
}

status_t bl_nor_decrypt_data(uint32_t addr, uint32_t size, uint32_t *data_start)
{
    return bl_nor_decrypt_data_with_img_ctx(addr, size, data_start, &s_img_gen_ctx);
}

status_t bl_nor_decrypt_data_prev_ctx(uint32_t addr, uint32_t size, uint32_t *data_start)
{
    return bl_nor_decrypt_data_with_img_ctx(addr, size, data_start, &s_img_gen_ctx_previous);
}

status_t bl_nor_decrypt_data_backup_ctx(uint32_t addr, uint32_t size, uint32_t *data_start)
{
    return bl_nor_decrypt_data_with_img_ctx(addr, size, data_start, &s_img_gen_ctx_backup);
}

void bl_nor_encrypt_region_refresh(uint32_t start, uint32_t bytes)
{
    for (uint32_t i = 0; i < 2; i++)
    {
        prot_region_block_info_t *plain_block_info = &s_img_gen_ctx.plain_block_info[i];
        prot_region_block_info_t *enc_block_info   = &s_img_gen_ctx.enc_block_info[i];
        if ((start <= KIB_ADDR(i)) && ((start + bytes) > KIB_ADDR(i)))
        {
            // Clear PRDB because it has been erased
            memset(plain_block_info, 0, sizeof(*plain_block_info));
            memset(enc_block_info, 0, sizeof(*enc_block_info));
        }
    }
}

status_t bl_nor_encrypt_split_prdb(void)
{
    status_t status = kStatus_Success;
    trng_config_t trng_config;
    prot_region_block_info_t *block0 = NULL;
    prot_region_block_info_t *block1 = NULL;
    uint32_t *ctx0                   = NULL;
    uint32_t *ctx1                   = NULL;
    uint32_t cryptoLen               = 0;

    // First, let's backup the current context
    status = SLN_AUTH_backup_context(kHabBootloader);

    if (kStatus_Success == status)
    {
        block0 = &s_img_gen_ctx.plain_block_info[kHabBootloader];
        block1 = &s_img_gen_ctx.plain_block_info[kHabApplication];

        DEBUG_LOG(("SUCCESS: Current context backed up, creating second context now...\r\n"));

        // Set both contexts to enabled
        s_img_gen_ctx.encrypt_enabled[kHabApplication] = s_img_gen_ctx.encrypt_enabled[kHabBootloader];

        // Just in case, but should definitely be set
        s_img_gen_ctx.bee_key_sel[kHabApplication].option = s_img_gen_ctx.bee_key_sel[kHabBootloader].option;

        // Copy index zero config to index one
        memcpy(block1, block0, sizeof(prot_region_block_info_t));

        // DEBUG_LOG(("Initial Bootloader context region: start - 0x%X, end - 0x%X\r\n",
        // block0->prdb.encrypt_region.start, block0->prdb.encrypt_region.end)); DEBUG_LOG(("Initial Application context
        // region: start - 0x%X, end - 0x%X\r\n", block1->prdb.encrypt_region.start, block1->prdb.encrypt_region.end));

        // Adjust addresses
        block0->prdb.encrypt_region.end   = bl_nor_encrypt_get_region_end(kHabBootloader);
        block1->prdb.encrypt_region.start = bl_nor_encrypt_get_region_start(kHabApplication);
        block1->prdb.encrypt_region.end   = bl_nor_encrypt_get_region_end(kHabApplication);

#if 0
        for (uint32_t idx = 0; idx < 16; idx++)
        {
            DEBUG_LOG(("%02X", block0->kib.aes_key[idx]));
        }
        DEBUG_LOG(("\r\n"));

        for (uint32_t idx = 0; idx < 16; idx++)
        {
            DEBUG_LOG(("%02X", block0->kib.iv[idx]));
        }
        DEBUG_LOG(("\r\n"));
#endif

        // DEBUG_LOG(("Bootloader context region: start - 0x%X, end - 0x%X\r\n", block0->prdb.encrypt_region.start,
        // block0->prdb.encrypt_region.end)); DEBUG_LOG(("Application context region: start - 0x%X, end - 0x%X\r\n",
        // block1->prdb.encrypt_region.start, block1->prdb.encrypt_region.end));

        // DEBUG_LOG(("Bootloader tagh -- 0x%X\r\n", block0->prdb.tagh));
        // DEBUG_LOG(("Bootloader tagl -- 0x%X\r\n", block0->prdb.tagl));

        // DEBUG_LOG(("Application tagh -- 0x%X\r\n", block1->prdb.tagh));
        // DEBUG_LOG(("Application tagl -- 0x%X\r\n", block1->prdb.tagl));

        loop_delay();

        for (uint32_t facIdx = 0; facIdx < block0->prdb.fac_region_count; facIdx++)
        {
            block0->prdb.fac_regions[facIdx].start = block0->prdb.encrypt_region.start;
            block0->prdb.fac_regions[facIdx].end   = block0->prdb.encrypt_region.end;
            block0->prdb.fac_regions[facIdx].mode  = 0U;
        }

        for (uint32_t facIdx = 0; facIdx < block1->prdb.fac_region_count; facIdx++)
        {
            block1->prdb.fac_regions[facIdx].start = block1->prdb.encrypt_region.start;
            block1->prdb.fac_regions[facIdx].end   = block1->prdb.encrypt_region.end;
            block1->prdb.fac_regions[facIdx].mode  = 0U;
        }

        // Initialize TRNG module before generating nonce and KIB
        status = TRNG_GetDefaultConfig(&trng_config);
    }

    if (kStatus_Success == status)
    {
        DEBUG_LOG(("SUCCESS: Initializing RNG...\r\n"));

        // Set sample mode of the TRNG ring oscillator to Von Neumann, for better random data.
        trng_config.sampleMode = kTRNG_SampleModeVonNeumann;
        status                 = TRNG_Init(TRNG, &trng_config);
    }

    if (kStatus_Success == status)
    {
        DEBUG_LOG(("SUCCESS: Generate key...\r\n"));

        // Generate random KIB info: aes_key
        status = TRNG_GetRandomData(TRNG, &block1->kib.aes_key, 16);
    }

    if (kStatus_Success == status)
    {
        DEBUG_LOG(("SUCCESS: Generate iv...\r\n"));

        // Generate random KIB info: initial vector
        status = TRNG_GetRandomData(TRNG, &block1->kib.iv, 16);

#if 0
        for (uint32_t idx = 0; idx < 16; idx++)
        {
            DEBUG_LOG(("%02X", block1->kib.aes_key[idx]));
        }
        DEBUG_LOG(("\r\n"));

        for (uint32_t idx = 0; idx < 16; idx++)
        {
            DEBUG_LOG(("%02X", block1->kib.iv[idx]));
        }
        DEBUG_LOG(("\r\n"));

        loop_delay();
#endif
    }

    if (kStatus_Success == status)
    {
        DEBUG_LOG(("SUCCESS: Encrypt ctx0...\r\n"));

        // Get encrypted config
        bl_nor_encrypt_get_config_block(kHabBootloader, &ctx0, &cryptoLen);

        if (NULL == ctx0)
        {
            DEBUG_LOG(("ERROR: Encryption of ctx0 failed!\r\n"));
            status = kStatus_Fail;
        }
    }

    if (kStatus_Success == status)
    {
        DEBUG_LOG(("SUCCESS: Encrypt ctx1...\r\n"));

        // Get encrypted config
        bl_nor_encrypt_get_config_block(kHabApplication, &ctx1, &cryptoLen);

        if (NULL == ctx1)
        {
            DEBUG_LOG(("ERROR: Encryption of ctx1 failed!\r\n"));
            status = kStatus_Fail;
        }
    }

    if (kStatus_Success == status)
    {
        DEBUG_LOG(("SUCCESS: Saving new contexts...\r\n"));

        loop_delay();

        // Save both contexts
        status = SLN_AUTH_full_save_context((uint32_t *)ctx0, (uint32_t *)ctx1);
    }

    return status;
}

status_t bl_nor_crypto_ctx_compare_backup(bool *isMatch, bool *isOriginal, uint32_t index)
{
    status_t status = kStatus_Success;

    if ((NULL == isMatch) || (NULL == isOriginal) || (index > 1))
    {
        status = kStatus_InvalidArgument;
    }

    if (kStatus_Success == status)
    {
        bool isWait = false; // Boolean to force entry into wait states

        DEBUG_LOG(("Setting up DCP...\r\n"));
        // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a crash

        // Hold execution here. This is useful for attaching debugger during encrypted XIP.
        while (isWait)
        {
            __asm("NOP");
        }

        dcp_alg_ctx_t dcp_ctx;
        dcp_aes_init(&dcp_ctx);

        prot_region_block_info_t plain_block_info;
        prot_region_block_info_t enc_block_info;
        aes_key_sel_t key_sel;

        switch (ROM_OCOTP_BEE_KEY0_SEL_VALUE())
        {
            case kBeeKeySel_OTPMK_SNVS_Low:
                key_sel.option = OTPMK_SNVS_LOW_FLAG_BE;
                break;
            case kBeeKeySel_OTPMK_SNVS_High:
                key_sel.option = OTPMK_SNVS_HIGH_FLAG_BE;
                break;
            case kBeeKeySel_SW_GP2:
                key_sel.option = SW_GP2_FLAG_BE;
                break;
            default:
                key_sel.key = NULL;
                break;
        }

        dcp_aes_set_key(&dcp_ctx, key_sel, 128);
        uint32_t kib_addr  = BACKUP_REGION + KIB_ADDR(index) - FlexSPI_AMBA_BASE;
        uint32_t prdb_addr = BACKUP_REGION + PRDB_ADDR(index) - FlexSPI_AMBA_BASE;

        // Read EKIB from Flash
        DEBUG_LOG(("Reading backup kib from 0x%X...\r\n", kib_addr));
        // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a crash
        status = SLN_Read_Flash_At_Address(kib_addr, (uint8_t *)&(enc_block_info.kib), sizeof(key_info_block_t));

        if (kStatus_Success == status)
        {
            DEBUG_LOG(("Reading backup prdb from 0x%X...\r\n", prdb_addr));
            // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
            // crash
            // Read EPRDB from flash
            status = SLN_Read_Flash_At_Address(prdb_addr, (uint8_t *)&(enc_block_info.prdb),
                                               sizeof(prot_region_desc_block_t));
        }

        if (kStatus_Success == status)
        {
            DEBUG_LOG(("Extracting backup KIB details...\r\n"));
            // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
            // crash
            // Decrypt EKIB
            dcp_aes_ecb_crypt(&dcp_ctx, kAesMode_Decrypt, (uint8_t *)&enc_block_info.kib,
                              (uint8_t *)&plain_block_info.kib, sizeof(key_info_block_t));

            DEBUG_LOG(("Extracting backup PRDB details...\r\n"));
            // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
            // crash

            // Decrypt PRDB
            aes_key_sel_t key_sel;
            key_sel.key = plain_block_info.kib.aes_key;
            dcp_aes_set_key(&dcp_ctx, key_sel, 128);
            dcp_aes_cbc_crypt(&dcp_ctx,                          // DCP context
                              kAesMode_Decrypt,                  // Decrypt
                              plain_block_info.kib.iv,           // Intial vector
                              (uint8_t *)&enc_block_info.prdb,   // Source, encrypted data
                              (uint8_t *)&plain_block_info.prdb, // Destination, decrypted data
                              sizeof(prot_region_desc_block_t)); // Size in bytes
        }

        if ((plain_block_info.prdb.tagh != BEE_PROT_REGION_BLK_TAGH) ||
            (plain_block_info.prdb.tagl != BEE_PROT_REGION_BLK_TAGL))
        {
            DEBUG_LOG(("Bad tags...\r\n"));
            // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
            // crash
            status = kStatus_Fail;
        }
        else
        {
            DEBUG_LOG(("Comparing backup with current...\r\n"));
            // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
            // crash
            bool keyDiff = (bool)memcmp(s_img_gen_ctx.plain_block_info[kHabBootloader].kib.aes_key,
                                        plain_block_info.kib.aes_key, 16);
            bool ivDiff =
                (bool)memcmp(s_img_gen_ctx.plain_block_info[kHabBootloader].kib.iv, plain_block_info.kib.iv, 16);
            bool ctrDiff  = (bool)memcmp(s_img_gen_ctx.plain_block_info[kHabBootloader].prdb.encrypt_region.counter,
                                        plain_block_info.prdb.encrypt_region.counter, 16);
            bool addrDiff = (s_img_gen_ctx.plain_block_info[kHabBootloader].prdb.encrypt_region.start !=
                             plain_block_info.prdb.encrypt_region.start) ||
                            (s_img_gen_ctx.plain_block_info[kHabBootloader].prdb.encrypt_region.end !=
                             plain_block_info.prdb.encrypt_region.end);
            bool origRegion =
                (plain_block_info.prdb.encrypt_region.end == bl_nor_encrypt_get_region_end(kHabApplication)) &&
                (plain_block_info.prdb.encrypt_region.start == bl_nor_encrypt_get_region_start(kHabBootloader));

            if (!keyDiff || !ivDiff || !ctrDiff)
            {
                DEBUG_LOG(("Crypto context match!\r\n"));
                // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
                // crash
                *isMatch = true;
            }
            else
            {
                DEBUG_LOG(("No crypto context match!\r\n"));
                // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
                // crash
                *isMatch = false;
            }

            if (addrDiff && origRegion)
            {
                DEBUG_LOG(("Original context found!\r\n"));
                // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
                // crash
                *isOriginal = true;
            }
            else
            {
                DEBUG_LOG(("Not original context\r\n"));
                // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a
                // crash
                *isOriginal = false;
            }
        }
    }

    return status;
}

status_t bl_nor_encrypt_ensure_context(void)
{
    status_t status = kStatus_Success;

    // Load current context from NVM
    status = bl_nor_encrypt_region_info_load_default();

    if (kStatus_Success == status)
    {
        DEBUG_LOG(("SUCCESS: Loaded crypto context from NVM.\r\n"));

        // Check for two PRDBs
        if (!is_valid_options(&s_img_gen_ctx.plain_block_info[kHabBootloader]))
        {
            DEBUG_LOG(("ERROR: No valid PRDB0!\r\n"));

            // There is no valid PRDB0, should always have PRDB0 from factory programming tool
            status = kStatus_Fail;
        }
    }

    if (kStatus_Success == status)
    {
        if (!is_valid_options(&s_img_gen_ctx.plain_block_info[kHabApplication]))
        {
            DEBUG_LOG(("WARNING: No valid PRDB1.\r\n"));

            // No valid PRDB1, we can now make one from PRDB0 (if key selects are equal)
            if (ROM_OCOTP_BEE_KEY0_SEL_VALUE() == ROM_OCOTP_BEE_KEY1_SEL_VALUE())
            {
                DEBUG_LOG(("KEY0_SEL = 0x%X\r\n", ROM_OCOTP_BEE_KEY0_SEL_VALUE()));
                DEBUG_LOG(("KEY1_SEL = 0x%X\r\n", ROM_OCOTP_BEE_KEY1_SEL_VALUE()));
                status = kStatus_ReadOnly;
            }
            else
            {
                DEBUG_LOG(("ERROR: Invalid key select configuration!\r\n"));
                status = kStatus_Fail;
            }
        }
    }

    return status;
}
