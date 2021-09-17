/*
 * Copyright (c) 2016-2017 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __CRYPTO_INTERNAL_H__
#define __CRYPTO_INTERNAL_H__

typedef struct bt_aes_128_cmac_state
{
    uint8_t k1[16];
    uint8_t k2[16];
    uint8_t x[16];
    uint8_t y[16];
    uint8_t key[16];
    uint8_t data[16];
    uint8_t len;
    bool busy;
} bt_aes_128_cmac_state_t;

int prng_init(void);

int bt_aes_128_cmac_be(const uint8_t *key, const uint8_t *in, size_t len,
           uint8_t *out);

int bt_aes_128_cmac_setup(bt_aes_128_cmac_state_t *state, const uint8_t key[16]);
int bt_aes_128_cmac_init(bt_aes_128_cmac_state_t *state);
int bt_aes_128_cmac_update(bt_aes_128_cmac_state_t *state, const uint8_t * data, size_t len);
int bt_aes_128_cmac_final(uint8_t tag[16], bt_aes_128_cmac_state_t *state);

#endif /* __CRYPTO_INTERNAL_H__ */
