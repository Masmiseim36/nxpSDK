/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "ncp_crc.h"
#include "ncp_tlv_adapter.h"
#include "ncp_adapter.h"
#include "fsl_os_abstraction.h"
#if CONFIG_NCP_USE_ENCRYPT
#include "mbedtls/gcm.h"
#include "ncp_cmd_common.h"
#include "mbedtls_common.h"
#endif
#include "ncp_cmd_system.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if CONFIG_NCP_UART
extern ncp_intf_ops_t ncp_uart_ops;
#elif (CONFIG_NCP_SPI)
extern ncp_intf_ops_t ncp_spi_ops;
#elif (CONFIG_NCP_USB)
extern ncp_intf_ops_t ncp_usb_ops;
#elif (CONFIG_NCP_SDIO)
extern ncp_intf_ops_t ncp_sdio_ops;
#endif

/* Global variable containing NCP internal statistics */
#if CONFIG_NCP_DEBUG
ncp_stats_t ncp_stats;
#endif

/* indicate interface status */
stats_inft_t ncp_intf;

ncp_tlv_adapter_t ncp_tlv_adapter;

/* NCP adapter TX variable*/
/* NCP adapter tx queue */
static osa_msgq_handle_t ncp_tlv_msgq_handle; /* ncp adapter TX msgq */
OSA_MSGQ_HANDLE_DEFINE(ncp_tlv_msgq_buff, NCP_TLV_QUEUE_LENGTH,  sizeof(void *));

/* NCP adapter tx mutex for queue counter*/
OSA_MUTEX_HANDLE_DEFINE(ncp_tlv_queue_mutex);
/* NCP adapter tx task */
static OSA_TASK_HANDLE_DEFINE(ncp_tlv_thread);

void ncp_tlv_process(osa_task_param_t arg);
static OSA_TASK_DEFINE(ncp_tlv_process, NCP_TLV_TX_TASK_PRIORITY, 1, NCP_TLV_TX_TASK_STACK_SIZE, 0);
/* NCP adapter tx queue counter */
static int ncp_tlv_queue_len = 0;
static bool ncp_initialized = false;

/*******************************************************************************
 * API
 ******************************************************************************/

static void ncp_tlv_cb(void *arg)
{
#ifdef CONFIG_NCP_SDIO
    extern power_cfg_t global_power_config;

    NCP_COMMAND *cmd = (NCP_COMMAND *)arg;
    if (!cmd)
    {
        ncp_adap_d("%s: cmd is NULL", __FUNCTION__);
        return;
    }

    ncp_adap_d("%s: cmd=0x%x SLEEP_CFM=0x%x wake_mode=%u", __FUNCTION__, cmd->cmd,
        NCP_CMD_SYSTEM_POWERMGMT_MCU_SLEEP_CFM, global_power_config.wake_mode);
    if (cmd->cmd == NCP_CMD_SYSTEM_POWERMGMT_MCU_SLEEP_CFM)
    {
        if (global_power_config.wake_mode == WAKE_MODE_GPIO)
        {
            if (NULL != ncp_tlv_adapter.intf_ops->pm_ops->enter)
            {
                ncp_tlv_adapter.intf_ops->pm_ops->enter(NCP_PM_STATE_PM3);
                ncp_adap_d("%s: lpm_enter PM3 done", __FUNCTION__);
            }
        }
    }
#endif
}

static void ncp_tlv_free_elmt(ncp_tlv_qelem_t **qbuf)
{
    OSA_MemoryFree(*qbuf);
    *qbuf = NULL;
    OSA_MutexLock((osa_mutex_handle_t)ncp_tlv_queue_mutex, osaWaitForever_c);
    if (ncp_tlv_queue_len <= 0)
        ncp_adap_e("invalid ncp tlv queue length: %d", NCP_TLV_QUEUE_LENGTH);
    else
        ncp_tlv_queue_len--;
    OSA_MutexUnlock(ncp_tlv_queue_mutex);
}

void ncp_tlv_process(osa_task_param_t arg)
{
    ncp_tlv_qelem_t *qbuf = NULL;
    while (1)
    {
        if( OSA_MsgQGet(ncp_tlv_msgq_handle, &qbuf, osaWaitForever_c) == KOSA_StatusSuccess)
        {
            NCP_TLV_STATS_INC(rx);
			/* sync send data */
            NCP_INTF_STATUS_SET(tx);
            ncp_tlv_adapter.intf_ops->send(qbuf->tlv_buf, qbuf->tlv_sz, ncp_tlv_cb);
            NCP_INTF_STATUS_CLEAR(tx);
			/* free element */
            ncp_tlv_free_elmt(&qbuf);
        }
        else
            NCP_TLV_STATS_INC(err);
    }
}

/*
    enqueue the qelement to ncp tx queue
*/
static ncp_status_t ncp_tlv_tx_enque(ncp_tlv_qelem_t *qelem)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;
    OSA_MutexLock(ncp_tlv_queue_mutex, osaWaitForever_c);
    if (ncp_tlv_queue_len == NCP_TLV_QUEUE_LENGTH)
    {
        ncp_adap_e("ncp tlv queue is full max queue length: %d", NCP_TLV_QUEUE_LENGTH);
        status = NCP_STATUS_QUEUE_FULL;
        goto Fail;
    }
    if (OSA_MsgQPut(ncp_tlv_msgq_handle, &qelem) != KOSA_StatusSuccess)
    {
        ncp_adap_e("ncp tlv enqueue failure");
        NCP_TLV_STATS_INC(err);
        status = NCP_STATUS_ERROR;
        goto Fail;
    }
    else
    {
        ncp_tlv_queue_len++;
		NCP_TLV_STATS_INC(tx);
        ncp_adap_d("enque tlv_buf success");
    }
Fail:
    OSA_MutexUnlock((osa_mutex_handle_t)ncp_tlv_queue_mutex);
    return status;
}


#if CONFIG_NCP_USE_ENCRYPT

int ncp_tlv_adapter_encrypt_init(const uint8_t *key_enc, const uint8_t *key_dec, 
                                 const uint8_t *iv_enc, const uint8_t *iv_dec,
                                 uint16_t key_len, uint16_t iv_len)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    int ret = 0, ret2 = 0;
    NCP_ASSERT(key_enc && key_dec && iv_enc && iv_dec);

    if ((adapter->crypt) || (key_len > NCP_ENDECRYPT_KEY_LEN) 
                         || (iv_len > NCP_ENDECRYPT_IV_LEN))
    {
        return NCP_STATUS_ERROR;
    }

    adapter->crypt = (crypt_param_t*)OSA_MemoryAllocate(sizeof(crypt_param_t));
    if (!adapter->crypt)
    {
        return NCP_STATUS_NOMEM;
    }
    (void) memset(adapter->crypt, 0, sizeof(crypt_param_t));

    adapter->crypt->gcm_ctx_enc = OSA_MemoryAllocate(sizeof(mbedtls_gcm_context));
    if (!adapter->crypt->gcm_ctx_enc)
    {
        (void) OSA_MemoryFree(adapter->crypt);
        adapter->crypt = NULL;
        return NCP_STATUS_NOMEM;
    }

    adapter->crypt->gcm_ctx_dec = OSA_MemoryAllocate(sizeof(mbedtls_gcm_context));
    if (!adapter->crypt->gcm_ctx_dec)
    {
        (void) OSA_MemoryFree(adapter->crypt->gcm_ctx_enc);
        (void) OSA_MemoryFree(adapter->crypt);
        adapter->crypt = NULL;
        return NCP_STATUS_NOMEM;
    }
    
    (void) memcpy(adapter->crypt->key_enc, key_enc, NCP_ENDECRYPT_KEY_LEN);
    (void) memcpy(adapter->crypt->key_dec, key_dec, NCP_ENDECRYPT_KEY_LEN);
    (void) memcpy(adapter->crypt->iv_enc, iv_enc, NCP_ENDECRYPT_IV_LEN);
    (void) memcpy(adapter->crypt->iv_dec, iv_dec, NCP_ENDECRYPT_IV_LEN);
    
    adapter->crypt->key_len = key_len;
    adapter->crypt->iv_len = iv_len;
    adapter->crypt->dec_buf = NULL;
    adapter->crypt->dec_buf_len = 0;
    
    (void) mbedtls_gcm_init((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_enc);
    (void) mbedtls_gcm_init((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_dec);
    
    ret = mbedtls_gcm_setkey((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_enc, 
                             MBEDTLS_CIPHER_ID_AES,
                             adapter->crypt->key_enc,
                             adapter->crypt->key_len * 8);
    ret2 = mbedtls_gcm_setkey((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_dec, 
                             MBEDTLS_CIPHER_ID_AES,
                             adapter->crypt->key_dec,
                             adapter->crypt->key_len * 8);
    if (ret != 0 || ret2 != 0)
    {
        (void) mbedtls_gcm_free((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_enc);
        (void) mbedtls_gcm_free((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_dec);
        (void) OSA_MemoryFree(adapter->crypt->gcm_ctx_enc);
        (void) OSA_MemoryFree(adapter->crypt->gcm_ctx_dec);
        (void) OSA_MemoryFree(adapter->crypt);
        adapter->crypt = NULL;
        return NCP_STATUS_ERROR;
    }
        
    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_encrypt_deinit(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;

    if (!adapter->crypt)
    {
        return NCP_STATUS_ERROR;
    }
    
    (void) mbedtls_gcm_free((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_enc);
    (void) memset(adapter->crypt->gcm_ctx_enc, 0, sizeof(mbedtls_gcm_context));
    (void) OSA_MemoryFree(adapter->crypt->gcm_ctx_enc);
    
    (void) mbedtls_gcm_free((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_dec);
    (void) memset(adapter->crypt->gcm_ctx_dec, 0, sizeof(mbedtls_gcm_context));
    (void) OSA_MemoryFree(adapter->crypt->gcm_ctx_dec);
    
    (void) OSA_MemoryFree(adapter->crypt->dec_buf);
    
    (void) memset(adapter->crypt, 0, sizeof(crypt_param_t));
    (void) OSA_MemoryFree(adapter->crypt);
    adapter->crypt = NULL;

    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_encrypt_enable(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    if (!adapter->crypt)
    {
        return NCP_STATUS_ERROR;
    }

    adapter->crypt->flag = 1;
    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_encrypt_disable(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    if (adapter->crypt)
    {
        adapter->crypt->flag = 0;
    }
    return NCP_STATUS_SUCCESS;
}

int ncp_tlv_adapter_is_encrypt_mode(void)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    return adapter->crypt && adapter->crypt->flag;
}

static ncp_status_t ncp_tlv_encrypt(void *input, void *output, size_t input_len)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    int ret = 0;  
    uint8_t tag_buf[16];
    NCP_ASSERT(input && output);

    if ((!adapter->crypt) || (!adapter->crypt->flag))
    {
        return NCP_STATUS_ERROR;
    }

    ret = mbedtls_gcm_crypt_and_tag((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_enc, 
                        MBEDTLS_GCM_ENCRYPT, input_len,
                        adapter->crypt->iv_enc, adapter->crypt->iv_len,
                        NULL, 0, input, output, sizeof(tag_buf), tag_buf);
    if (ret != 0)
    {
        ncp_adap_e("mbedtls_gcm_crypt_and_tag err %d\r\n", ret);
        return NCP_STATUS_ERROR;
    }

    return NCP_STATUS_SUCCESS;
}

static ncp_status_t ncp_tlv_decrypt(void *input, size_t input_len)
{
    ncp_tlv_adapter_t *adapter = &ncp_tlv_adapter;
    int ret = 0;
    uint8_t tag_buf[16];

    if ((!adapter->crypt) || (!adapter->crypt->flag))
    {
        return NCP_STATUS_ERROR;
    }
    if (adapter->crypt->dec_buf_len < input_len)
    {
        if (adapter->crypt->dec_buf)
        {
            (void) OSA_MemoryFree(adapter->crypt->dec_buf);
        }
        
        adapter->crypt->dec_buf = (uint8_t*)OSA_MemoryAllocate(input_len);
        
        if (!adapter->crypt->dec_buf)
        {
            adapter->crypt->dec_buf_len = 0;
            return NCP_STATUS_NOMEM;
        }
        
        adapter->crypt->dec_buf_len = input_len;
    }
    
    /* MbedTLS gcm decryption requires input and output buffer be different,
       so decrypt input to dec_buf and then copy output data to input */
    ret = mbedtls_gcm_crypt_and_tag((mbedtls_gcm_context*)adapter->crypt->gcm_ctx_dec, 
                        MBEDTLS_GCM_DECRYPT, input_len,
                        adapter->crypt->iv_dec, adapter->crypt->iv_len,
                        NULL, 0, input, adapter->crypt->dec_buf, 
                        sizeof(tag_buf), tag_buf);
    if (ret != 0)
    {
        return NCP_STATUS_ERROR;
    }

    (void) memcpy(input, adapter->crypt->dec_buf, input_len);

    return NCP_STATUS_SUCCESS;
}

#endif  /* CONFIG_NCP_USE_ENCRYPT */


/*
    qbuf_len = sizeof(ncp_tlv_qelem_t) + sdio_intf_head + tlv_sz + chksum_len
    qbuf_tlv = qbuf + sizeof(ncp_tlv_qelem_t)
    memcpy_buf = qbuf + sizeof(ncp_tlv_qelem_t) + sdio_intf_head
    chksum_buf = qbuf + sizeof(ncp_tlv_qelem_t) + sdio_intf_head + tlv_sz
    sdio_intf_head: reserved length for sdio interface header
*/
ncp_status_t ncp_tlv_send(void *tlv_buf, size_t tlv_sz)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;
    ncp_tlv_qelem_t *qbuf = NULL;
    uint8_t *qbuf_tlv = NULL, *chksum_buf = NULL;
    uint16_t qlen = 0, sdio_intf_head = 0, chksum_len = 4;
    uint32_t chksum = 0;
#if CONFIG_NCP_SDIO
    sdio_intf_head = 4;
#endif
    qlen = sizeof(ncp_tlv_qelem_t) + sdio_intf_head + tlv_sz + chksum_len;
    qbuf = (ncp_tlv_qelem_t *)OSA_MemoryAllocate(qlen);
    if (!qbuf)
    {
        NCP_TLV_STATS_INC(drop);
        ncp_adap_d("%s: failed to allocate memory for tlv queue element", __FUNCTION__);
        return NCP_STATUS_NOMEM;
    }
    qbuf->tlv_sz = tlv_sz + chksum_len;
    qbuf->priv = NULL;
    qbuf_tlv = (uint8_t *)qbuf + sizeof(ncp_tlv_qelem_t);

#if !CONFIG_NCP_USE_ENCRYPT
    (void) memcpy(qbuf_tlv + sdio_intf_head, tlv_buf, tlv_sz);
#else
    if ((!ncp_tlv_adapter.crypt) || (!ncp_tlv_adapter.crypt->flag))
    {
        (void) memcpy(qbuf_tlv + sdio_intf_head, tlv_buf, tlv_sz);
    }
    else
    {
        struct _NCP_CMD_HEADER *cmd_hdr = (struct _NCP_CMD_HEADER *)tlv_buf;
        if (ncp_cmd_is_data_cmd(cmd_hdr->cmd))
        {
            (void) memcpy(qbuf_tlv + sdio_intf_head, tlv_buf, tlv_sz);
        }
        else
        {
            (void) memcpy(qbuf_tlv + sdio_intf_head, tlv_buf, TLV_CMD_HEADER_LEN);
            if (tlv_sz > TLV_CMD_HEADER_LEN)
            {
                status = ncp_tlv_encrypt(tlv_buf + TLV_CMD_HEADER_LEN,
                                qbuf_tlv + sdio_intf_head + TLV_CMD_HEADER_LEN, 
                                tlv_sz - TLV_CMD_HEADER_LEN);
                if (status != NCP_STATUS_SUCCESS)
                {
                    NCP_TLV_STATS_INC(drop);
                    ncp_adap_e("ncp tlv encrypt err %d", (int)status);
                    return NCP_STATUS_ERROR;
                }
            }
        }
    }
#endif /* CONFIG_NCP_USE_ENCRYPT */

    qbuf->tlv_buf = qbuf_tlv;
    chksum = ncp_tlv_chksum(qbuf_tlv + sdio_intf_head, (uint16_t)tlv_sz);
    chksum_buf = qbuf_tlv + sdio_intf_head + tlv_sz;
    chksum_buf[0] = chksum & 0xff;
    chksum_buf[1] = (chksum & 0xff00) >> 8;
    chksum_buf[2] = (chksum & 0xff0000) >> 16;
    chksum_buf[3] = (chksum & 0xff000000) >> 24;

    status = ncp_tlv_tx_enque(qbuf);
    if(status != NCP_STATUS_SUCCESS)
    {
        ncp_adap_e("ncp tlv enque element fail");
        if (qbuf)
        {
            OSA_MemoryFree(qbuf);
            qbuf = NULL;
        }
    }

    return status;
}

static ncp_status_t ncp_tlv_tx_init(void)
{
    if (OSA_MutexCreate(ncp_tlv_queue_mutex) != KOSA_StatusSuccess)
    {
        ncp_adap_e("%s, ncp tx mutex create fail");
        return NCP_STATUS_ERROR;
    }
    ncp_tlv_msgq_handle = (osa_msgq_handle_t)ncp_tlv_msgq_buff;
    if (OSA_MsgQCreate(ncp_tlv_msgq_handle, NCP_TLV_QUEUE_LENGTH,  sizeof(void *)) != KOSA_StatusSuccess)
    {
        ncp_adap_e("ncp tx msg queue create fail");
        return NCP_STATUS_ERROR;
    }
    (void)OSA_TaskCreate((osa_task_handle_t)ncp_tlv_thread, OSA_TASK(ncp_tlv_process), NULL);
    ncp_adap_d("ncp tx init success");
    return NCP_STATUS_SUCCESS;
}

static void ncp_tlv_tx_deinit(void)
{
    ncp_tlv_qelem_t *qbuf = NULL;
    if (OSA_TaskDestroy(ncp_tlv_thread) != KOSA_StatusSuccess)
    {
        ncp_adap_e("ncp adapter tx deint Task fail");
    }
    while (1)
    {
        if( OSA_MsgQGet(ncp_tlv_msgq_handle, &qbuf, 0) == KOSA_StatusSuccess)
        {
            /* free element */
            ncp_tlv_free_elmt(&qbuf);
        }
        else
        {
            ncp_adap_d("ncp adapter queue flush completed");
            break;
        }
    }
    OSA_MutexLock(ncp_tlv_queue_mutex, osaWaitForever_c);
    ncp_tlv_queue_len = 0;
    OSA_MutexUnlock(ncp_tlv_queue_mutex);
    if (OSA_MutexDestroy(ncp_tlv_queue_mutex) != KOSA_StatusSuccess)
    {
        ncp_adap_e("ncp adapter tx deint queue mutex fail");
    }
    if (OSA_MsgQDestroy(ncp_tlv_msgq_handle) != KOSA_StatusSuccess)
    {
        ncp_adap_e("ncp adapter tx deint MsgQ fail");
    }
}

ncp_status_t ncp_adapter_init(void)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;

    if (ncp_initialized == true)
    {
        return status;
    }

#if CONFIG_NCP_UART
    ncp_tlv_adapter.intf_ops = &ncp_uart_ops;
#elif (CONFIG_NCP_SPI)
    ncp_tlv_adapter.intf_ops = &ncp_spi_ops;
#elif (CONFIG_NCP_USB)
    ncp_tlv_adapter.intf_ops = &ncp_usb_ops;
#elif (CONFIG_NCP_SDIO)
    ncp_tlv_adapter.intf_ops = &ncp_sdio_ops;
#endif
    /* Init CRC32 */
    ncp_tlv_chksum_init();
    status = ncp_tlv_tx_init();
    if (status != NCP_STATUS_SUCCESS)
    {
        return status;
    }
    /* Init interface */
    status = (ncp_status_t)ncp_tlv_adapter.intf_ops->init(NULL);
    if (status != NCP_STATUS_SUCCESS)
    {
        ncp_adap_e("ncp adapater init fail");
        return status;
    }

    NCP_INTF_STATUS_SET(ready);
    ncp_initialized = true;

    return status;
}

ncp_status_t ncp_adapter_deinit(void)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;

    ncp_initialized = false;

    /* Deinit interface */
    status = (ncp_status_t)ncp_tlv_adapter.intf_ops->deinit(NULL);

    ncp_tlv_adapter.intf_ops = NULL;
    ncp_tlv_tx_deinit();
    return status;
}

void ncp_tlv_install_handler(uint8_t class, void *func_cb)
{
    NCP_ASSERT((uint8_t)NCP_MAX_CLASS > class);
    NCP_ASSERT(NULL != func_cb);

    ncp_tlv_adapter.tlv_handler[class] = (tlv_callback_t)func_cb;
}

void ncp_tlv_uninstall_handler(uint8_t class)
{
    NCP_ASSERT((uint8_t)NCP_MAX_CLASS > class);

    ncp_tlv_adapter.tlv_handler[class] = NULL;
}

void ncp_tlv_dispatch(void *tlv, size_t tlv_sz)
{
    ncp_status_t status = NCP_STATUS_SUCCESS;
    uint32_t local_checksum = 0, remote_checksum = 0;
    uint8_t class = 0;

    ncp_adap_d("Receive TLV command, dispatch it!");

    /* check CRC */
    remote_checksum = NCP_GET_PEER_CHKSUM((uint8_t *)tlv, tlv_sz);
    local_checksum  = ncp_tlv_chksum(tlv, tlv_sz);
    if (remote_checksum != local_checksum)
    {
        status = NCP_STATUS_CHKSUMERR;
        /* Send NCP_EVENT_CRC_CHECK_ERROR  */
        uint8_t crc_check_error_event[TLV_CMD_HEADER_LEN] = {0x0f, 0x00, 0x32, 0x40, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        ncp_tlv_send(crc_check_error_event, TLV_CMD_HEADER_LEN);

        ncp_adap_e("Checksum validation failed!");
        return;
    }

#if CONFIG_NCP_USE_ENCRYPT
    if ((ncp_tlv_adapter.crypt) && (ncp_tlv_adapter.crypt->flag != 0)
                              && (tlv_sz > TLV_CMD_HEADER_LEN))
    {
        struct _NCP_CMD_HEADER *cmd_hdr = (struct _NCP_CMD_HEADER *)tlv;
        if (!ncp_cmd_is_data_cmd(cmd_hdr->cmd))
        {
            status = ncp_tlv_decrypt(tlv + TLV_CMD_HEADER_LEN, tlv_sz - TLV_CMD_HEADER_LEN);
            if (status != NCP_STATUS_SUCCESS)
            {
                ncp_adap_e("ncp tlv decrypt err %d", (int)status);
                return;
            }
        }
    }
#endif /* CONFIG_NCP_USE_ENCRYPT */

    /* TLV command class */
    class = NCP_GET_CLASS(*((uint32_t *)tlv));
    if (ncp_tlv_adapter.tlv_handler[class])
        ncp_tlv_adapter.tlv_handler[class](tlv, tlv_sz, status);
}
