/*
 * Copyright 2021 NXP
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <errno/errno.h>

#include <porting.h>
#include <sys/byteorder.h>
#include <sys/atomic.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/crypto.h>

#include "bt_pal_hci_core.h"
#include "bt_pal_crypto_internal.h"

#ifndef CONFIG_BT_AES_128_ENCRYPT_SW
#define CONFIG_BT_AES_128_ENCRYPT_SW 1
#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */

#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#if CONFIG_BT_AES_128_ENCRYPT_SW
#include "mbedtls/aes.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context rng_ctx;
#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */
#else
#include "BT_common.h"
#include "BT_version.h"
#include "BT_hci_api.h"
#include "BT_smp_api.h"
#include "BT_sm_api.h"
#include "smp_pl.h"
#endif /* CONFIG_BT_SMP */

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_HCI_CORE)
#define LOG_MODULE_NAME bt_crypto
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#if CONFIG_BT_AES_128_ENCRYPT_SW
#else
#define HCI_CMD_TIMEOUT      BT_SECONDS(5)

struct bt_le_encrypt_rsp_cb_data
{
	/** The command OpCode that the buffer contains */
	uint8_t *enc_data;

	/** HCI status of the command completion */
	uint8_t  status;

	/** Encryption command counter is a out parameter which species the
	 * encryption counter. This parameter is used by inter stack
	 * to keep track each encryption command initiated and response
	 * received for each of them.
	 */
	uint8_t counter;

	/** Used by bt_le_encrypt_rsp_cb */
	osa_semaphore_handle_t sync;
	OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
};

static void bt_le_encrypt_rsp_cb(struct net_buf *buf, void *cb_param)
{
	struct bt_le_encrypt_rsp_cb_data *cb_data;
	struct bt_hci_rp_le_encrypt *rsp;
	struct bt_hci_evt_cmd_complete *evt;
	uint8_t counter;

	if (NULL == cb_param)
	{
		return;
	}

	cb_data = (struct bt_le_encrypt_rsp_cb_data *)cb_param;

	evt = (struct bt_hci_evt_cmd_complete *)buf->data;
	counter = evt->ncmd;

	rsp = (struct bt_hci_rp_le_encrypt *)&buf->data[sizeof(*evt)];

	if (counter == cb_data->counter)
	{
		memcpy(cb_data->enc_data, rsp->enc_data, sizeof(rsp->enc_data));

		if (NULL != cb_data->sync)
		{
			cb_data->status = rsp->status;
			OSA_SemaphorePost(cb_data->sync);
		}
	}
}
#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */

static int bt_aes_128_encrypt(const uint8_t in[16],
                     const uint8_t key[16],
                     uint8_t out[16])
{
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#if CONFIG_BT_AES_128_ENCRYPT_SW
	mbedtls_aes_context ctx;

	mbedtls_aes_init(&ctx);

	if(0 != mbedtls_aes_setkey_enc(&ctx, (const unsigned char *)key, 128))
	{
		return -1;
	}

	if(0 != mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, (const unsigned char *)in, (unsigned char *)out))
	{
		return -1;
	}

	mbedtls_aes_free(&ctx);

    return 0;
#else
    struct bt_hci_cmd_le_encrypt_rp_cb cb;
    struct bt_le_encrypt_rsp_cb_data cb_data;
    int err;
    uint8_t tmpKey[16];
    uint8_t tmpIn[16];
    osa_status_t ret;
    API_RESULT retval;
    uint8_t status;

    BT_DBG("key %s in %s", bt_hex(key, 16), bt_hex(in, 16));

    sys_memcpy_swap(tmpKey, key, 16);
    sys_memcpy_swap(tmpIn, in, 16);

    memset(&cb, 0, sizeof(cb));
    memset(&cb_data, 0, sizeof(cb_data));

    cb.cb = bt_le_encrypt_rsp_cb;
    cb.user_data = (void *)&cb_data;

    /* Register LE Encrypt rsp callback */
    err = hci_cmd_le_encrypt_rp_cb_register(&cb);
    if (0 != err)
    {
            return err;
    }

    ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)(cb_data.semaphoreHandle), 0);
    if (KOSA_StatusSuccess != ret)
    {
            return -ENOBUFS;
    }
    cb_data.sync = (osa_semaphore_handle_t)(cb_data.semaphoreHandle);
    cb_data.status = BT_HCI_ERR_UNSPECIFIED;
    cb_data.enc_data = out;

	/* Send HCI LE Enscrypt request. Invoke HCI Encrypt. */
    retval = BT_smp_128B_encrypt_pl ((UCHAR *)tmpKey, (UCHAR *)tmpIn, (UCHAR *)&cb_data.counter);

#ifdef BT_PAL_CRYPTO_DEBUG
    PRINTF("+++CRYPTO++%d\r\n", cb_data.counter);
#endif

    if (API_SUCCESS == retval)
    {
		/* Waiting for command complete */
		ret = OSA_SemaphoreWait(cb_data.sync, HCI_CMD_TIMEOUT);
#if 0
		assert(KOSA_StatusSuccess == ret);
#endif
    }

    (void)OSA_SemaphoreDestroy(cb_data.sync);
    (void)hci_cmd_le_encrypt_rp_cb_unregister(&cb);

    status = cb_data.status;
    if (status) {
            BT_WARN("status 0x%02x", status);
            switch (status) {
            case BT_HCI_ERR_CONN_LIMIT_EXCEEDED:
                    return -ECONNREFUSED;
            default:
                    return -EIO;
            }
    }
    sys_mem_swap(out, 16);
    return 0;
#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */
#else
    return -1;
#endif /* CONFIG_BT_SMP */
}

#if CONFIG_BT_AES_128_ENCRYPT_SW
#else
#ifdef BT_PAL_CRYPTO_DEBUG
static void bt_le_encrypt_rsp_monitor_cb(struct net_buf *buf, void *cb_param)
{
	struct bt_hci_evt_cmd_complete *evt;
	uint8_t counter;

	if (NULL == cb_param)
	{
		return;
	}

	evt = (struct bt_hci_evt_cmd_complete *)buf->data;
	counter = evt->ncmd;

    PRINTF("-----%d\r\n", counter);
}

void bt_le_encrypt_monitor(void)
{
    static struct bt_hci_cmd_le_encrypt_rp_cb cb;
    static struct bt_le_encrypt_rsp_cb_data cb_data;

	memset(&cb, 0, sizeof(cb));
	memset(&cb_data, 0, sizeof(cb_data));

	cb.cb = bt_le_encrypt_rsp_monitor_cb;
	cb.user_data = (void *)&cb_data;

	/* Register LE Encrypt rsp callback */
	(void)hci_cmd_le_encrypt_rp_cb_register(&cb);
}
#endif /* BT_PAL_CRYPTO_DEBUG */
#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */

static int prng_reseed()
{
    uint8_t seed[8];
    int ret, i;

    for (i = 0; i < (sizeof(seed) / 8); i++) {
            struct bt_hci_rp_le_rand *rp;
            struct net_buf *rsp;

            ret = bt_hci_cmd_send_sync(BT_HCI_OP_LE_RAND, NULL, &rsp);
            if (ret) {
                    return ret;
            }

            rp = (struct bt_hci_rp_le_rand *)rsp->data;
            memcpy(&seed[i * 8], rp->rand, 8);

            net_buf_unref(rsp);
    }
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#if CONFIG_BT_AES_128_ENCRYPT_SW

	mbedtls_entropy_init(&entropy);

	mbedtls_ctr_drbg_init(&rng_ctx);

	if(0 != mbedtls_ctr_drbg_seed(&rng_ctx, mbedtls_entropy_func, &entropy, NULL, 0))
	{
		return -1;
	}

#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */
#else
    srand(*((uint32_t *)seed));
#endif /* CONFIG_BT_SMP */
    return 0;
}

int prng_init(void)
{
#if ((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP))
#if CONFIG_BT_AES_128_ENCRYPT_SW
#else
#ifdef BT_PAL_CRYPTO_DEBUG
    bt_le_encrypt_monitor();
#endif
#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */
#endif /* CONFIG_BT_SMP */
    /* Check first that HCI_LE_Rand is supported */
    if (!BT_CMD_TEST(bt_dev.supported_commands, 27, 7)) {
            return -ENOTSUP;
    }

    return prng_reseed();
}

int bt_rand(void *buf, size_t len)
{
    uint32_t rng;

    for (size_t index = 0; index < len; index+=sizeof(rng))
    {
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#if CONFIG_BT_AES_128_ENCRYPT_SW
		if(0 != mbedtls_ctr_drbg_random(&rng_ctx, (unsigned char *)&rng, 4))
		{
			return -1;
		}
#endif /* CONFIG_BT_AES_128_ENCRYPT_SW */
#else
        rng = (uint32_t)rand();
#endif /* CONFIG_BT_SMP */
        for (size_t i = 0; i < MIN(len, (len - index));i++)
        {
            ((uint8_t *)buf)[index + i] = ((uint8_t *)&rng)[i];
        }
    }

    return 0;
}

int bt_encrypt_le(const uint8_t key[16], const uint8_t plaintext[16],
		  uint8_t enc_data[16])
{
	int err;
	uint8_t tmpKey[16];
	uint8_t tmpPlaintext[16];

	BT_DBG("key %s plaintext %s", bt_hex(key, 16), bt_hex(plaintext, 16));

	sys_memcpy_swap(tmpKey, key, 16);
	sys_memcpy_swap(tmpPlaintext, plaintext, 16);

	err = bt_aes_128_encrypt(tmpPlaintext, tmpKey, enc_data);

	if (0 == err)
	{
		sys_mem_swap(enc_data, 16);
		BT_DBG("enc_data %s", bt_hex(enc_data, 16));
	}
	else
	{
		BT_ERR("AES 128 Encrypt failed (error %d)", err);
	}

	return err;
}

int bt_encrypt_be(const uint8_t key[16], const uint8_t plaintext[16],
		  uint8_t enc_data[16])
{
	BT_DBG("key %s plaintext %s", bt_hex(key, 16), bt_hex(plaintext, 16));

	bt_aes_128_encrypt(plaintext, key, enc_data);

	BT_DBG("enc_data %s", bt_hex(enc_data, 16));

	return 0;
}

int bt_aes_128_cmac_be(const uint8_t *key, const uint8_t *in, size_t len,
			   uint8_t *out)
{
	bt_aes_128_cmac_state_t state;

	BT_DBG("key %s in %s", bt_hex(key, 16), bt_hex(in, len));

	bt_aes_128_cmac_setup(&state, key);
	bt_aes_128_cmac_update(&state, in, len);
	bt_aes_128_cmac_final(out, &state);

	BT_DBG("out %s", bt_hex(out, 16));

	return 0;
}

static void bt_crypto_left_shift_one_bit(uint8_t *input,
                                   uint8_t *output)
{
    int32_t i;
    uint8_t overflow = 0u;

    for ( i=15; i>=0; i-- )
    {
        output[i] = input[i] << 1u;
        output[i] |= overflow;
        overflow = ((input[i] & 0x80u) > 0u) ? 1u : 0u;
    }
}

static void bt_crypto_xor128(uint8_t *a,
                          const uint8_t *b,
                          uint8_t *out)
{
    uint32_t i;

    for (i=0u;i<16u; i++)
    {
      out[i] = a[i] ^ b[i];
    }
}

static void bt_crypto_padding(uint8_t *lastb,
                           uint8_t *pad,
                           uint32_t length)
{
    uint32_t j;

    /* original last block */
    for ( j=0u; j<16u; j++ ) {
        if ( j < length ) {
            pad[j] = lastb[j];
        } else if ( j == length ) {
            pad[j] = 0x80u;
        } else {
            pad[j] = 0x00u;
        }
    }
}

int bt_aes_128_cmac_setup(bt_aes_128_cmac_state_t *state, const uint8_t key[16])
{
    uint8_t const_Rb[16] = {0x00u, 0x00u, 0x00u, 0x00u,
                            0x00u, 0x00u, 0x00u, 0x00u,
                            0x00u, 0x00u, 0x00u, 0x00u,
                            0x00u, 0x00u, 0x00u, 0x87u};
    uint8_t L[16] = { 0 };
    uint8_t Z[16] = { 0 };
    uint8_t tmp[16] = {0};

	if (NULL == state)
	{
		return -EINVAL;
	}

	memset(state, 0, sizeof(*state));

	memcpy(state->key, key, sizeof(state->key));

	memset(Z, 0, sizeof(Z));
	bt_aes_128_encrypt(Z,key,L);

	if ( (L[0] & 0x80u) == 0u )
    {
        /* If MSB(L) = 0, then K1 = L << 1 */
        bt_crypto_left_shift_one_bit(L,state->k1);
    }
    else
    {
        /* Else K1 = ( L << 1 ) (+) Rb */
        bt_crypto_left_shift_one_bit(L,tmp);
        bt_crypto_xor128(tmp,const_Rb,state->k1);
    }

    if ( (state->k1[0] & 0x80u) == 0u )
    {
        bt_crypto_left_shift_one_bit(state->k1,state->k2);
    }
    else
    {
        bt_crypto_left_shift_one_bit(state->k1,tmp);
        bt_crypto_xor128(tmp,const_Rb,state->k2);
    }
	return 0;
}

int bt_aes_128_cmac_init(bt_aes_128_cmac_state_t *state)
{
	if (true == state->busy)
	{
		return -EAGAIN;
	}

	state->len = 0u;
	memset(state->x, 0, sizeof(state->x));
	memset(state->y, 0, sizeof(state->y));
	state->busy = true;
	return 0;
}

int bt_aes_128_cmac_update(bt_aes_128_cmac_state_t *state, const uint8_t * data, size_t len)
{
	uint32_t n;
	uint32_t i;
	const uint8_t * input;

	n = 0;

	if ((state->len + len) > sizeof(state->data))
	{
		memcpy(&state->data[state->len], data, sizeof(state->data) - state->len);
		len -= sizeof(state->data) - state->len;
		input = &data[sizeof(state->data) - state->len];
		state->len = sizeof(state->data);
		n = (uint8_t) ((len) / sizeof(state->data)); /* n is number of rounds */
		if (sizeof(state->data) == len)
		{
			n = 0;
		}
	}
	else
	{
		memcpy(&state->data[state->len], data, len);
		state->len += len;
		return 0;
	}

	if (state->len > 0u)
	{
		bt_crypto_xor128(state->x, &state->data[0u], state->y); /* Y := Mi (+) X  */
		bt_aes_128_encrypt(state->y, state->key, state->x); /* X := AES-128(KEY, Y) */
		state->len = 0;
	}

	if (n > 0)
	{
		if (0 == (len - n * sizeof(state->data)))
		{
			n = n - 1;
		}
	}

	for (i = 0u; i < n; i++)
    {
        bt_crypto_xor128(state->x, &input[sizeof(state->data) * i], state->y); /* Y := Mi (+) X  */
        bt_aes_128_encrypt(state->y, state->key, state->x); /* X := AES-128(KEY, Y) */
    }

	state->len = len - n * sizeof(state->data);
	if (state->len > 0)
	{
		memcpy(state->data, &input[n * sizeof(state->data)], state->len);
	}

	return 0;
}

int bt_aes_128_cmac_final(uint8_t tag[16], bt_aes_128_cmac_state_t *state)
{
    uint8_t M_last[16] = {0};
	uint8_t padded[16] = {0};

	if (sizeof(state->data) == state->len)
	{
		bt_crypto_xor128(&state->data[0], state->k1, M_last);
	}
	else
	{
        bt_crypto_padding(&state->data[0], padded, state->len);
        bt_crypto_xor128(padded, state->k2, M_last);
	}
    bt_crypto_xor128(state->x, M_last, state->y);
    bt_aes_128_encrypt(state->y, state->key, state->x);

	memcpy(tag, state->x, 16u);

	state->busy = false;
	return 0;
}
