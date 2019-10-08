/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include <stdint.h>
#include "dcp/fsl_dcp.h"
#include <string.h>

#define DCP_CTRL0_INTRRUPT_SHIFT 0
#define DCP_CTRL0_INTRRUPT_MASK (1 << DCP_CTRL0_INTRRUPT_SHIFT)
#define DCP_CTRL0_INTRRUPT(x) (((x) << DCP_CTRL0_INTRRUPT_SHIFT) & DCP_CTRL0_INTRRUPT_MASK)
#define DCP_CTRL0_DECR_SEMAPHORE_SHIFT 1
#define DCP_CTRL0_DECR_SEMAPHORE_MASK (1 << DCP_CTRL0_DECR_SEMAPHORE_SHIFT)
#define DCP_CTRL0_DECR_SEMAPHORE(x) (((x) << DCP_CTRL0_DECR_SEMAPHORE_SHIFT) & DCP_CTRL0_DECR_SEMAPHORE_MASK)
#define DCP_CTRL0_CHAIN_SHIFT 2
#define DCP_CTRL0_CHAIN_CONTIGUOUS_SHIFT 3
#define DCP_CTRL0_ENABLE_MEMCPY_SHIFT 4
#define DCP_CTRL0_ENABLE_CIPHER_SHIFT 5
#define DCP_CTRL0_ENABLE_CIPHER_MASK (1 << DCP_CTRL0_ENABLE_CIPHER_SHIFT)
#define DCP_CTRL0_ENABLE_CIPHER(x) (((x) << DCP_CTRL0_ENABLE_CIPHER_SHIFT) & DCP_CTRL0_ENABLE_CIPHER_MASK)
#define DCP_CTRL0_ENABLE_HASH_SHIFT 6
#define DCP_CTRL0_ENABLE_BLIT_SHIFT 7
#define DCP_CTRL0_CIPHER_ENCRYPT_SHIFT 8
#define DCP_CTRL0_CIPHER_ENCRYPT_MASK (1 << DCP_CTRL0_CIPHER_ENCRYPT_SHIFT)
#define DCP_CTRL0_CIPHER_ENCRYPT(x) (((x) << DCP_CTRL0_CIPHER_ENCRYPT_SHIFT) & DCP_CTRL0_CIPHER_ENCRYPT_MASK)
#define DCP_CTRL0_CIPHER_INIT_SHIFT 9
#define DCP_CTRL0_CIPHER_INIT_MASK (1 << DCP_CTRL0_CIPHER_INIT_SHIFT)
#define DCP_CTRL0_CIPHER_INIT(x) (((x) << DCP_CTRL0_CIPHER_INIT_SHIFT) & DCP_CTRL0_CIPHER_INIT_MASK)
#define DCP_CTRL0_OTP_KEY_SHIFT 10
#define DCP_CTRL0_OTP_KEY_MASK (1 << DCP_CTRL0_OTP_KEY_SHIFT)
#define DCP_CTRL0_OTP_KEY(x) (((x) << DCP_CTRL0_OTP_KEY_SHIFT) & DCP_CTRL0_OTP_KEY_MASK)
#define DCP_CTRL0_PAYLOAD_KEY_SHIFT 11
#define DCP_CTRL0_PAYLOAD_KEY_MASK (1 << DCP_CTRL0_PAYLOAD_KEY_SHIFT)
#define DCP_CTRL0_PAYLOAD_KEY(x) (((x) << DCP_CTRL0_PAYLOAD_KEY_SHIFT) & DCP_CTRL0_PAYLOAD_KEY_MASK)
#define DCP_CTRL0_HASH_INIT_SHIFT 12
#define DCP_CTRL0_HASH_TERM_SHIFT 13
#define DCP_CTRL0_CHECK_HASH_SHIFT 14
#define DCP_CTRL0_HASH_OUTPUT_SHIFT 15
#define DCP_CTRL0_CONSTANT_FILL_SHIFT 16
#define DCP_CTRL0_TEST_SEMA_IRQ_SHIFT 17
#define DCP_CTRL0_KEY_BYTESWAP_SHIFT 18
#define DCP_CTRL0_KEY_BYTESWAP_MASK (1 << DCP_CTRL0_KEY_BYTESWAP_SHIFT)
#define DCP_CTRL0_KEY_BYTESWAP(x) (((x) << DCP_CTRL0_KEY_BYTESWAP_SHIFT) & DCP_CTRL0_KEY_BYTESWAP_MASK)
#define DCP_CTRL0_KEY_WORDSWAP_SHIFT 19
#define DCP_CTRL0_KEY_WORDSWAP_MASK (1 << DCP_CTRL0_KEY_WORDSWAP_SHIFT)
#define DCP_CTRL0_KEY_WORDSWAP(x) (((x) << DCP_CTRL0_KEY_WORDSWAP_SHIFT) & DCP_CTRL0_KEY_WORDSWAP_MASK)
#define DCP_CTRL0_INPUT_BYTESWAP_SHIFT 20
#define DCP_CTRL0_INPUT_WORDSWAP_SHIFT 21
#define DCP_CTRL0_OUTPUT_BYTESWAP_SHIFT 22
#define DCP_CTRL0_OUTPUT_WORDSWAP_SHIFT 23
#define DCP_CTRL0_TAG_SHIFT 24

#define DCP_CTRL1_CIPHER_SELECT_SHIFT (0)
#define DCP_CTRL1_CIPHER_SELECT_WIDTH (4)
#define DCP_CTRL1_CIPHER_SELECT_MASK (((1 << DCP_CTRL1_CIPHER_SELECT_WIDTH) - 1) << DCP_CTRL1_CIPHER_SELECT_SHIFT)
#define DCP_CTRL1_CIPHER_SELECT(x) (((x) << DCP_CTRL1_CIPHER_SELECT_SHIFT) & DCP_CTRL1_CIPHER_SELECT_MASK)
#define DCP_CTRL1_CIPHER_MODE_SHIFT (4)
#define DCP_CTRL1_CIPHER_MODE_WIDTH (4)
#define DCP_CTRL1_CIPHER_MODE_MASK (((1 << DCP_CTRL1_CIPHER_MODE_WIDTH) - 1) << DCP_CTRL1_CIPHER_MODE_SHIFT)
#define DCP_CTRL1_CIPHER_MODE(x) (((x) << DCP_CTRL1_CIPHER_MODE_SHIFT) & DCP_CTRL1_CIPHER_MODE_MASK)
#define DCP_CTRL1_KEY_SELECT_SHIFT (8)
#define DCP_CTRL1_KEY_SELECT_WIDTH (8)
#define DCP_CTRL1_KEY_SELECT_MASK (((1 << DCP_CTRL1_KEY_SELECT_WIDTH) - 1) << DCP_CTRL1_KEY_SELECT_SHIFT)
#define DCP_CTRL1_KEY_SELECT(x) (((x) << DCP_CTRL1_KEY_SELECT_SHIFT) & DCP_CTRL1_KEY_SELECT_MASK)

typedef struct
{
    uint32_t *next;
    uint32_t ctrl0;
    uint32_t ctrl1;
    uint32_t *src;
    uint32_t *dst;
    uint32_t buffer_size;
    uint32_t *payload;
    uint32_t status;
} dcp_desc_t;

typedef struct
{
    uint32_t VAL;
    uint32_t SET;
    uint32_t CLR;
    uint32_t TOG;
} dcp_reg_t;

typedef struct
{
    volatile dcp_reg_t CTRL;          //!< Offset 0x00
    volatile dcp_reg_t STAT;          //!< Offset 0x10
    volatile dcp_reg_t CHANNELCTRL;   //!< Offset 0x20
    volatile dcp_reg_t CAPABILITY[2]; //!< Offset 0x30
    volatile dcp_reg_t CONTEXT;       //!< Offset 0x50
    volatile dcp_reg_t KEY;           //!< Offset 0x60
    volatile dcp_reg_t KEYDATA;       //!< Offset 0x70
    volatile dcp_reg_t PACKET[7];     //!< Offset 0x80
    uint32_t reserved0[4];
    volatile struct
    {
        dcp_reg_t CMDPTR;
        dcp_reg_t SEMA;
        dcp_reg_t STAT;
        dcp_reg_t OPTS;
    } CH[4]; //!< Offset 0x100

    uint32_t reserved13[128];

    volatile dcp_reg_t DBGSELECT;
    volatile dcp_reg_t DBGDATA;
    volatile dcp_reg_t PAGETABLE;
    volatile dcp_reg_t VERSION;
} DCP_TYPE;

#define NXP_DCP ((volatile DCP_TYPE *)DCP_BASE)

#define KEY_SEL_INDEX 0
#define PAYLOAD_FLAG 0xfedc3210
#define UNIQUE_KEY_FLAG 0xfedcfedc
#define INVALID_KEY_FLAG 0xfedcffff
#define KEY0_FLAG 0xfedc0000
#define KEY1_FLAG 0xfedc0001
#define KEY2_FLAG 0xfedc0002
#define KEY3_FLAG 0xfedc0003
#define AES_KEY_FLAG KEY3_FLAG
#define AES_KEY_INDEX 3
#define AES_CHN_INDEX 3

#define DCP_CHN_FLG(chn) (1 << (chn))
#define DCP_TIMEOUT_MAX 0xF0000000u

#define IOMUXC_GPR_DCP_KEY_SEL_MASK (1U << 4)
#define IOMUXC_GPR_DCP_KEY_MUX_SEL_MASK (1U << 4)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
// Enable DCP module
static void dcp_enable(void);
// Format B0, follow  A2.1 in  NIST Special Publication 800-38C
static void aes_ccm_format_b0(
    uint8_t *block, const uint8_t *iv, uint32_t iv_len, uint32_t mac_len, uint32_t aad_len, uint32_t input_len);
// Format Ctr0, follow  A2.1 in  NIST Special Publication 800-38C
static void aes_ccm_format_ctr0(uint8_t *ctr, const uint8_t *iv, uint8_t iv_len);
// Convert uint32 data to big-endian array
static void uint32_to_be(uint8_t *dst, uint32_t len, uint32_t number);
// Increment big-endian unsigned integer
static void num_increment_be(uint8_t *big_num, uint32_t bytes);

// Do AES-CCM encryption/decryption and authentication
static int aes_ccm_auth_crypt(dcp_alg_ctx_t *ctx,
                              aes_mode_t mode,
                              uint32_t input_len,
                              const uint8_t *iv,
                              uint32_t iv_len,
                              const uint8_t *aad,
                              uint32_t aad_len,
                              const uint8_t *input,
                              uint8_t *output,
                              uint8_t *mac,
                              uint32_t mac_len);
// Configure AES KEY and IV based on key selection
static void dcp_aes_config_key_iv(dcp_alg_ctx_t *ctx, dcp_desc_t *dcp_desc, uint32_t *payload, uint8_t *iv);

// Wait until the DCP operation completes or reaches timesout parameter
static int dcp_wait_task_done(const dcp_desc_t *dcp_desc);

/*******************************************************************************
 * Codes
 ******************************************************************************/
static void dcp_enable(void)
{
    if (NXP_DCP->CTRL.VAL & DCP_CTRL_SFTRST_MASK)
    {
        NXP_DCP->CTRL.CLR = DCP_CTRL_SFTRST_MASK | DCP_CTRL_CLKGATE_MASK;
    }

    NXP_DCP->CTRL.SET = DCP_CHN_FLG(AES_CHN_INDEX);        // Enable Channel  interrupt
    NXP_DCP->CHANNELCTRL.SET = DCP_CHN_FLG(AES_CHN_INDEX); // Enable Channel
}

// See nxp_dcp.h for more details
int dcp_aes_init(dcp_alg_ctx_t *ctx)
{
    int status;

    if (!ctx)
    {
        status = kAesOperation_InvalidArgument;
    }
    else
    {
        if (!(CCM->CCGR0 & CCM_CCGR0_CG5_MASK))
        {
            CCM->CCGR0 |= CCM_CCGR0_CG5_MASK;
        }
        memset(ctx, 0, sizeof(*ctx));

        dcp_enable();

        status = kAesOperation_Success;
    }

    return status;
}

// See nxp_dcp.h for more details
int dcp_aes_set_key(dcp_alg_ctx_t *ctx, const aes_key_sel_t key_sel, uint32_t keybits)
{
    int status;
    uint32_t i;
    uint32_t key_buffer[4];

    if ((!ctx) || (!key_sel.key) || (keybits != 128))
    {
        status = kAesOperation_InvalidArgument;
    }
    else
    {
        status = kAesOperation_Success;
        ctx->_private[KEY_SEL_INDEX] = key_sel.option;
        switch (key_sel.option)
        {
            case KEY0_FLAG:
            case KEY1_FLAG:
            case KEY2_FLAG:
                /* Extract KEY index from the key address */
                ctx->_private[KEY_SEL_INDEX] = key_sel.option & 0x03;
                break;
            default:
                memcpy(&key_buffer, key_sel.key, sizeof(key_buffer));
                NXP_DCP->KEY.VAL = DCP_KEY_INDEX(AES_KEY_INDEX) | DCP_KEY_SUBWORD(0);
                ctx->_private[KEY_SEL_INDEX] = AES_KEY_FLAG;
                for (i = 0; i < 4; i++)
                {
                    NXP_DCP->KEYDATA.VAL = key_buffer[i];
                }
                // Wipe out key in normal memory immediately
                memset(&key_buffer, 0, sizeof(key_buffer));
                break;
            case OTPMK_SNVS_LOW_FLAG_LE:
            case OTPMK_SNVS_LOW_FLAG_BE:
                IOMUXC_GPR->GPR10 &= (uint32_t)~IOMUXC_GPR_DCP_KEY_MUX_SEL_MASK;
                IOMUXC_GPR->GPR3 &= (uint32_t)~IOMUXC_GPR_DCP_KEY_SEL_MASK;
                dcp_entry();
                break;
            case OTPMK_SNVS_HIGH_FLAG_LE:
            case OTPMK_SNVS_HIGH_FLAG_BE:
                IOMUXC_GPR->GPR10 &= (uint32_t)~IOMUXC_GPR_DCP_KEY_MUX_SEL_MASK;
                IOMUXC_GPR->GPR3 |= (uint32_t)IOMUXC_GPR_DCP_KEY_SEL_MASK;
                dcp_entry();
                break;
            case SW_GP2_FLAG_LE:
            case SW_GP2_FLAG_BE:
                IOMUXC_GPR->GPR10 |= IOMUXC_GPR_DCP_KEY_MUX_SEL_MASK;
                dcp_entry();
                break;
            case UNIQUE_KEY_FLAG:
                break;
        }
    }

    return status;
}

void dcp_aes_config_key_iv(dcp_alg_ctx_t *ctx, dcp_desc_t *dcp_desc, uint32_t *payload, uint8_t *iv)
{
    uint32_t key_flag = ctx->_private[KEY_SEL_INDEX];
    uint32_t iv_offset = 0;
    switch (key_flag)
    {
        case AES_KEY_FLAG:
            dcp_desc->ctrl1 |= DCP_CTRL1_KEY_SELECT(AES_KEY_INDEX);
            break;
        case KEY0_FLAG:
        case KEY1_FLAG:
        case KEY2_FLAG:
            /* Extract KEY index from flag */
            dcp_desc->ctrl1 |= DCP_CTRL1_KEY_SELECT(key_flag & 0x03);
            break;
        case OTPMK_SNVS_HIGH_FLAG_LE:
        case SW_GP2_FLAG_LE:
        case OTPMK_SNVS_LOW_FLAG_LE:
            dcp_desc->ctrl0 |= DCP_CTRL0_OTP_KEY(1);
            dcp_desc->ctrl1 |= DCP_CTRL1_KEY_SELECT(0xFF);
            break;
        case OTPMK_SNVS_HIGH_FLAG_BE:
        case SW_GP2_FLAG_BE:
        case OTPMK_SNVS_LOW_FLAG_BE:
            dcp_desc->ctrl0 |= DCP_CTRL0_OTP_KEY(1) | DCP_CTRL0_KEY_BYTESWAP(1) | DCP_CTRL0_KEY_WORDSWAP(1);
            dcp_desc->ctrl1 |= DCP_CTRL1_KEY_SELECT(0xFF);
            break;
        case UNIQUE_KEY_FLAG:
            dcp_desc->ctrl0 |= DCP_CTRL0_OTP_KEY(1);
            dcp_desc->ctrl1 |= DCP_CTRL1_KEY_SELECT(0xFE);
            break;
        default:
            break;
    }

    if (iv != NULL)
    {
        memcpy(&payload[iv_offset], iv, 16);
    }

    dcp_desc->payload = (iv != NULL) ? payload : NULL;
    dcp_desc->status = 0;
}

int dcp_wait_task_done(const dcp_desc_t *dcp_desc)
{
    uint32_t timeout = DCP_TIMEOUT_MAX;
    int status = kAesOperation_Success;

    __DSB(); // Do memory barrier, so that the dcp_desc, payload will be always updated prior to do AES operation
    if (SCB->CCR & SCB_CCR_DC_Msk)
    {
        // Flush Dcahe, so DCP can get the real data from source memory
        SCB_CleanDCache();
    }
    __ISB();

    // Wait until this operation completes.
    NXP_DCP->CH[AES_CHN_INDEX].CMDPTR.VAL = (uint32_t)dcp_desc;
    NXP_DCP->CH[AES_CHN_INDEX].SEMA.VAL = 1;
    while (((NXP_DCP->STAT.VAL & DCP_CHN_FLG(AES_CHN_INDEX)) == 0x00) && timeout--)
    {
    }
    if ((NXP_DCP->CH[AES_CHN_INDEX].STAT.VAL & 0xFF) != 0)
    {
        NXP_DCP->CH[AES_CHN_INDEX].STAT.CLR = 0xFF;
        status = kAesOperation_Error;
    }
    NXP_DCP->STAT.CLR = DCP_CHN_FLG(AES_CHN_INDEX);

    if ((timeout < 1) && (status != kAesOperation_Error))
    {
        status = kAesOperation_Timeout;
    }
    __DSB(); // Do memory barrier, so that the dcp_desc, payload will be always updated prior to do AES operation
    if (SCB->CCR & SCB_CCR_DC_Msk)
    {
        // Invalidate Dcache, so ARM core can get the real data from destination memory
        SCB_InvalidateDCache();
    }
    __ISB();

    return status;
}

// See nxp_dcp.h for more details
int dcp_aes_ecb_crypt(dcp_alg_ctx_t *ctx, aes_mode_t mode, uint8_t *input, uint8_t *output, uint32_t length)
{
    int status;

    if ((!ctx) || (mode > kAesMode_Encrypt) || (!input) || (!output) || (length & 0x0F))
    {
        status = kAesOperation_InvalidArgument;
    }
    else
    {
        dcp_desc_t dcp_desc;
        uint32_t payload[8];

        dcp_desc.next = NULL;
// AES ECB mode
#ifdef CPU_MIMXRT1011CAE4A
        dcp_desc.ctrl0 = DCP_CTRL0_CIPHER_INIT(1) | DCP_CTRL0_ENABLE_CIPHER(1) | DCP_CTRL0_DECR_SEMAPHORE(1) |
                         DCP_CTRL0_INTRRUPT(1) | DCP_CTRL0_CIPHER_ENCRYPT(mode) |
                         DCP_CTRL0_KEY_WORDSWAP(OTPMK_SNVS_LOW_FLAG_LE == ctx->_private[KEY_SEL_INDEX] ||
                                                OTPMK_SNVS_HIGH_FLAG_LE ==
                                                    ctx->_private[KEY_SEL_INDEX]); // 0 - Decryption, 1- Encryption
#else
        dcp_desc.ctrl0 = DCP_CTRL0_CIPHER_INIT(1) | DCP_CTRL0_ENABLE_CIPHER(1) | DCP_CTRL0_DECR_SEMAPHORE(1) |
                         DCP_CTRL0_INTRRUPT(1) | DCP_CTRL0_CIPHER_ENCRYPT(mode); // 0 - Decryption, 1- Encryption
#endif
        dcp_desc.ctrl1 = 0; // AES ECB mode
        dcp_desc.src = (uint32_t *)input;
        dcp_desc.dst = (uint32_t *)output;
        dcp_desc.buffer_size = length;

        dcp_aes_config_key_iv(ctx, &dcp_desc, payload, NULL);

        // Wait until this operation completes.
        status = dcp_wait_task_done(&dcp_desc);
    }

    return status;
}

void dcp_entry(void)
{
    NXP_DCP->CTRL.SET = DCP_CTRL_SFTRST_MASK;
    dcp_enable();
}
