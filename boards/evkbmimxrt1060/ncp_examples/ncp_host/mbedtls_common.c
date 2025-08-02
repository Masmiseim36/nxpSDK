/** @file mbedtls_common.c
 *
 *  @brief This file provides NCP common mbedtls interfaces.
 *
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "ncp_debug.h"
#include "ncp_cmd_system.h"
#include "ncp_tlv_adapter.h"
#if CONFIG_NCP_USE_ENCRYPT
#include "mbedtls_common.h"
#include "fsl_trng.h"
#include "key_cert.h"
#include "ncp_cmd_common.h"
#include "ncp_cmd_ble.h"
#ifdef RW610
#include "mbedtls/platform_time.h"
#include "fsl_rtc.h"
#endif

/* TODO: remove this private definition and use the definition in header file */
#define _NCP_CMD_WLAN                0x00000000
#define _NCP_CMD_WLAN_SOCKET         0x00900000

static OSA_SEMAPHORE_HANDLE_DEFINE(evt_recv_sem);

static const int _ciphersuite_list[] = { 
    CONFIG_TLS_CIPHERSUITE,
    0 
}; 

mbedtls_ctx_t *_mbedtls;
uint32_t _verify_num;

static int port_mbedtls_rng(void *p_rng, unsigned char *buf, size_t len)
{
    if (!buf)
    {
        return -1;
    }
    for (uint32_t i = 0; i < (uint32_t)len; ++i)
    {
        buf[i] = (unsigned char)rand();
    }
    return 0;
}

static void* port_mbedtls_calloc(size_t blk_num, size_t blk_size)
{
    uint32_t byte_num = blk_num * blk_size;
    /* OSA_MemoryAllocate internally clear these memory */
    void *p = OSA_MemoryAllocate(byte_num);
    if (!p)
    {
        ncp_e("**** calloc %d bytes failed\r\n", byte_num);
        ncp_e("**** free mem size %d\r\n", xPortGetFreeHeapSize());
    }
    return p;
}

static void port_mbedtls_free(void *p)
{
    if (p)
    {
        (void) OSA_MemoryFree(p);
    }
}

static int port_mbedtls_recv(void *ctx, unsigned char *buf, size_t len)
{
    uint16_t recv_data_cnt = 0;
    int ret = 0;
    NCP_ASSERT(_mbedtls);

    for (uint32_t i = 0; i < 50; ++i)
    {
        (void) OSA_SemaphoreWait(evt_recv_sem, 100);
        
        recv_data_cnt = ((_mbedtls->ringbuf.in + NCP_MBEDTLS_RECV_BUF_LEN) - 
                              _mbedtls->ringbuf.out) % NCP_MBEDTLS_RECV_BUF_LEN;
        if (recv_data_cnt >= len)
        {
            (void) memcpy(buf, _mbedtls->ringbuf.dat + _mbedtls->ringbuf.out, len);
            _mbedtls->ringbuf.out = (_mbedtls->ringbuf.out + len) % NCP_MBEDTLS_RECV_BUF_LEN;
            ret = len;
            break;
        }
    }
  
    return ret;
}

static int port_mbedtls_entropy_read(unsigned char *buf, unsigned int buf_len)
{
    NCP_ASSERT(_mbedtls);

    if (buf_len > MBEDTLS_ENTROPY_BLOCK_SIZE)
    {
        ncp_d("**** error read size is too large %d", buf_len);
        return 0;
    }
    
    (void) memcpy(buf, _mbedtls->entropy_buf, buf_len);
    
    return buf_len;
}

static int port_mbedtls_entropy_write(unsigned char *buf, unsigned int buf_len)
{
    NCP_ASSERT(_mbedtls);

    if (buf_len > MBEDTLS_ENTROPY_BLOCK_SIZE)
    {
        ncp_e("**** write size is too large %d\n", buf_len);
        return 0;
    }
    
    (void) memcpy(_mbedtls->entropy_buf, buf, buf_len);
    
    return buf_len;
}

static void port_mbedtls_dbgmsg_output(void *arg, int dbg_level, 
                        const char *file, int line, const char *str)
{
    (void)PRINTF("%s", str);
}

static int port_mbedtls_export_keys(void *ctx, const unsigned char *master, 
    const unsigned char *keyblk, size_t mac_key_len, size_t keylen, size_t iv_copy_len)
{
    NCP_ASSERT(master && keyblk);
    int ret = 0;
    uint16_t i = 0;
    const uint8_t *key1 = (uint8_t*)keyblk + mac_key_len * 2;
    const uint8_t *key2 = key1 + keylen;
    const uint8_t *iv1 = (uint8_t*)key2 + keylen;
    const uint8_t *iv2 = iv1 + iv_copy_len;

    _verify_num = 0;
    for (i = 0; i < keylen * 2; i += 4)
    {
        _verify_num += *(uint32_t*)&key1[i];
    }
    for (i = 0; i < iv_copy_len * 2; i += 4)
    {
        _verify_num += *(uint32_t*)&iv1[i];
    }

    // ncp_dump_hex(key1, keylen);
    // ncp_dump_hex(key2, keylen);
    // ncp_dump_hex(iv1, iv_copy_len);
    // ncp_dump_hex(iv2, iv_copy_len);

    if (_mbedtls->is_server)
    {
        ret = ncp_tlv_adapter_encrypt_init(key1, key2, iv1, iv2, keylen, iv_copy_len);
    }
    else
    {
        ret = ncp_tlv_adapter_encrypt_init(key2, key1, iv2, iv1, keylen, iv_copy_len);
    }
    if (ret != NCP_STATUS_SUCCESS)
    {
        ncp_e("ncp_tlv_adapter_encrypt_init err %d", ret);
    }

    ncp_d("*** export key %d\r\n", ret);

    return 0;
}

static int ncp_encrypt_init_mbedtls(void)
{
    NCP_ASSERT(_mbedtls);
    int ret = 0;
    trng_config_t trngConfig;
    uint32_t seed = 0;

    (void) TRNG_GetDefaultConfig(&trngConfig);
    (void) TRNG_Init(TRNG, &trngConfig);
    (void) TRNG_GetRandomData(TRNG, &seed, sizeof(seed));
    (void) srand(seed);
    (void) port_mbedtls_rng(NULL, (unsigned char *)(_mbedtls->entropy_buf), sizeof(_mbedtls->entropy_buf));
    mbedtls_ssl_config_init(&_mbedtls->conf);
    // mbedtls_platform_set_printf(&DbgConsole_Printf);
#ifdef MBEDTLS_DEBUG_C
    mbedtls_debug_set_threshold(CONFIG_NCP_MBEDTLS_DBG_LEVEL);  
#endif
    mbedtls_ssl_conf_dbg(&_mbedtls->conf, port_mbedtls_dbgmsg_output, NULL);
    (void) mbedtls_platform_set_calloc_free(&port_mbedtls_calloc, &port_mbedtls_free);
    mbedtls_ssl_init(&_mbedtls->ssl);
    mbedtls_ssl_conf_rng(&_mbedtls->conf, port_mbedtls_rng, &_mbedtls->ctr_drbg);
    mbedtls_x509_crt_init(&_mbedtls->ca_cert);
    mbedtls_x509_crt_init(&_mbedtls->own_cert);
    mbedtls_pk_init(&_mbedtls->pkey);
    mbedtls_entropy_init(&_mbedtls->entropy);
    mbedtls_ctr_drbg_init(&_mbedtls->ctr_drbg);
#if !defined(MBEDTLS_SSL_CLI_ALLOW_WEAK_CERTIFICATE_VERIFICATION_WITHOUT_HOSTNAME)
    mbedtls_ssl_set_hostname(&_mbedtls->ssl, "localhost");
#endif
#if defined(MBEDTLS_ENTROPY_NV_SEED) && defined(MBEDTLS_PLATFORM_NV_SEED_ALT)
    (void) mbedtls_platform_set_nv_seed(&port_mbedtls_entropy_read, &port_mbedtls_entropy_write);
#endif
    mbedtls_ssl_conf_export_keys_cb(&_mbedtls->conf, &port_mbedtls_export_keys, NULL);
    if (psa_crypto_init() != PSA_SUCCESS)
    {
        ncp_e("psa_crypto_init err\n");
        ret = -TLS_ERR_PSA_INIT;
        goto exit;
    }

    ret = mbedtls_x509_crt_parse(&_mbedtls->ca_cert, (const unsigned char *)CA_CERT,
                                    sizeof(CA_CERT));
    if (ret != 0)
    {
        ncp_e("mbedtls_x509_crt_parse ca_cert err %d\n\n", ret);
        ret = -TLS_ERR_PARSE_CERT;
        goto exit;
    }

    ret = mbedtls_x509_crt_parse(&_mbedtls->own_cert, (const unsigned char *)OWN_CERT,
                                    sizeof(OWN_CERT));
    if (ret != 0)
    {
        ncp_e("mbedtls_x509_crt_parse own_cert err %d\n\n", ret);
        ret = -TLS_ERR_PARSE_CERT;
        goto exit;
    }

    ret = mbedtls_pk_parse_key(&_mbedtls->pkey, (const unsigned char *)OWN_PVTKEY,
                                sizeof(OWN_PVTKEY), TLS_KEY_PASSWORD, TLS_KEY_PASSWORD_LEN);
    if (ret != 0)
    {
        ncp_e("mbedtls_pk_parse_key pkey err %d\n\n", ret);
        ret = -TLS_ERR_PARSE_KEY;
        goto exit;
    }

    ret = mbedtls_ssl_config_defaults(&_mbedtls->conf,
                    _mbedtls->is_server ? MBEDTLS_SSL_IS_SERVER : MBEDTLS_SSL_IS_CLIENT,
                    MBEDTLS_SSL_TRANSPORT_STREAM,
                    MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0)
    {
        ncp_e("mbedtls_ssl_config_defaults err %d\n\n", ret);
        ret = -TLS_ERR_SSL_INIT;
        goto exit;
    }

    mbedtls_ssl_conf_ca_chain(&_mbedtls->conf, &_mbedtls->ca_cert, NULL);
    mbedtls_ssl_conf_authmode(&_mbedtls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    ret = mbedtls_ssl_conf_own_cert(&_mbedtls->conf, &_mbedtls->own_cert, &_mbedtls->pkey);
    if (ret != 0)
    {
        ncp_e("mbedtls_ssl_conf_own_cert err %d\n\n", ret);
        ret = -TLS_ERR_CFG_CERT;
        goto exit;
    }
    ret = mbedtls_ssl_setup(&_mbedtls->ssl, &_mbedtls->conf);
    if (ret != 0)
    {
        ncp_e("mbedtls_ssl_setup err %d\n\n", ret);
        ret = -TLS_ERR_SSL_SETUP;
        goto exit;
    }
    (void) mbedtls_ssl_session_reset(&_mbedtls->ssl);    
    mbedtls_ssl_set_bio(&_mbedtls->ssl, NULL, &port_mbedtls_send, &port_mbedtls_recv, NULL);
    mbedtls_ssl_conf_ciphersuites(&_mbedtls->conf, &_ciphersuite_list[0]);
    
    ret = TLS_OK;
     
exit:    
    ncp_d("*** mbedtls init exit with %d\n", ret);
    return ret;
}

int ncp_encrypt_setup(uint8_t is_server)
{
    int ret = 0;

    ncp_d("**** ncp_encrypt_setup called\r\n");
    ncp_d("**** os free mem size %d\r\n", xPortGetFreeHeapSize());

    ret = OSA_SemaphoreCreateBinary(evt_recv_sem);
    if (ret != 0)
    {
        ncp_e("Failed to create encryptrecv semaphore: %d", ret);
        return -TLS_ERR_CRETAE_SEM;
    }

    _mbedtls = (mbedtls_ctx_t*)OSA_MemoryAllocate(sizeof(mbedtls_ctx_t));
    if (!_mbedtls)
    {
        ncp_e("Failed to alloc mem for _mbedtls");
        return -TLS_ERR_ALLOC_MEM;
    }
    (void) memset(_mbedtls, 0, sizeof(*_mbedtls));
    _mbedtls->is_server = is_server ? 1 : 0;

    /* If ncp_encrypt_init_mbedtls returns fail, the allocated 
        resource is released by calling ncp_encrypt_teardown later */
    ret = ncp_encrypt_init_mbedtls();
    if (ret != 0)
    {
        ncp_e("ncp_encrypt_init_mbedtls fail %d", ret);
        return -TLS_ERR_INIT_TLS;
    }

    ncp_d("**** ncp_encrypt_setup exit succ\r\n");
    return TLS_OK;
}

int ncp_encrypt_teardown(void)
{
    int ret = TLS_OK;

    if (_mbedtls)
    {
        mbedtls_ssl_free(&_mbedtls->ssl);
        mbedtls_ssl_config_free(&_mbedtls->conf);
        mbedtls_ctr_drbg_free(&_mbedtls->ctr_drbg);
        mbedtls_entropy_free(&_mbedtls->entropy);
        mbedtls_x509_crt_free(&_mbedtls->ca_cert);
        mbedtls_x509_crt_free(&_mbedtls->own_cert);
        mbedtls_pk_free(&_mbedtls->pkey);
        mbedtls_psa_crypto_free();

        (void) OSA_MemoryFree(_mbedtls);
        _mbedtls = NULL;
    }

    (void) OSA_SemaphoreDestroy(evt_recv_sem);

    ncp_d("**** os free mem size %d\r\n", xPortGetFreeHeapSize());
    ncp_d("**** ncp_encrypt_teardown\r\n");
    return ret;
}

int ncp_encrypt_process_handshake_data(uint8_t *data, uint16_t len)
{
    uint16_t recv_data_cnt = 0;
    uint16_t recv_buf_free_len = 0;
    uint16_t tmp = 0;
    uint16_t cp_len = 0;

    if (!_mbedtls)
    {
        ncp_w("ncp encrypt _mbedtls is NULL");
        return -TLS_ERR_HANDSHAKING_NOT_START;
    }

    if (!data || !len)
    {
        ncp_e("data or len is 0");
        return -TLS_ERR_INVALID_PARAM;
    }

    recv_data_cnt = ((_mbedtls->ringbuf.in + NCP_MBEDTLS_RECV_BUF_LEN) - 
                                        _mbedtls->ringbuf.out) % NCP_MBEDTLS_RECV_BUF_LEN;
    recv_buf_free_len = NCP_MBEDTLS_RECV_BUF_LEN - recv_data_cnt - 1;
    if (!recv_buf_free_len)
    {
        ncp_w("ncp encrypt handshake data, recv_buf_free_len", recv_buf_free_len);
        return -TLS_ERR_RINGBUF_FULL;
    }

    if (recv_buf_free_len < len) 
    {
        ncp_w("ncp mbedtls recv drop data %d - %d", len, recv_buf_free_len);
        len = recv_buf_free_len;
    }

    if (_mbedtls->ringbuf.in >= _mbedtls->ringbuf.out)
    {
        tmp = NCP_MBEDTLS_RECV_BUF_LEN - _mbedtls->ringbuf.in;
        cp_len = tmp >= len ? len : tmp;

        (void) memcpy(_mbedtls->ringbuf.dat + _mbedtls->ringbuf.in, data, cp_len);
        _mbedtls->ringbuf.in = (_mbedtls->ringbuf.in + cp_len) % NCP_MBEDTLS_RECV_BUF_LEN;

        data += cp_len;
        len -= cp_len;
    }

    if (len)
    {
        (void) memcpy(_mbedtls->ringbuf.dat + _mbedtls->ringbuf.in, data, len);
        _mbedtls->ringbuf.in += cp_len;
        len -= cp_len;
    }

    (void) OSA_SemaphorePost(&evt_recv_sem);
    return TLS_OK;
}

int ncp_cmd_is_data_cmd(uint32_t cmd)
{
    return (GET_CMD_CLASS(cmd) == _NCP_CMD_WLAN 
                    && GET_CMD_SUBCLASS(cmd) == _NCP_CMD_WLAN_SOCKET)
        || (cmd == NCP_CMD_BLE_L2CAP_SEND || cmd == NCP_EVENT_L2CAP_RECEIVE);
}

#endif /* CONFIG_NCP_USE_ENCRYPT */

#ifdef RW610
#ifdef MBEDTLS_PLATFORM_TIME_ALT
mbedtls_time_t ncp_time_func(mbedtls_time_t *timer)
{
    (void)(timer);
    return 0;
}
#endif

void ncp_set_mbedtls_set_time()
{
#ifdef MBEDTLS_PLATFORM_TIME_ALT
    mbedtls_platform_set_time(ncp_time_func);
#endif
}

#ifdef MBEDTLS_PLATFORM_GMTIME_FUNC_USE
struct tm *mbedtls_platform_gmtime_r(const mbedtls_time_t *tt,
                                     struct tm *tm_buf)
{
    (void)(tt);
    rtc_datetime_t date;

    /* Get date time */
    RTC_GetDatetime(RTC, &date);

    tm_buf->tm_sec = date.second;
    tm_buf->tm_min = date.minute;
    tm_buf->tm_hour = date.hour;
    tm_buf->tm_mday = date.day;
    tm_buf->tm_mon = date.month - 1;
    tm_buf->tm_year = date.year - 1900;

    return tm_buf;
}
#endif
#endif
