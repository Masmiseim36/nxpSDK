/** @file mbedtls_host.c
 *
 *  @brief This file provides NCP host mbedtls interfaces.
 *
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "ncp_debug.h"
#include "ncp_cmd_system.h"
#include "ncp_host_command.h"
#include "ncp_host_app.h"
#include "ncp_tlv_adapter.h"
#include "ncp_host_command_wifi.h"
#include "ncp_cmd_wifi.h"
#include "ksdk_mbedtls.h"
#include <string.h>


#if CONFIG_NCP_USE_ENCRYPT

#include "mbedtls_common.h"

static void ncp_encrypt_handshake_task(void *pvParameters);
#define NCP_HOST_ENCRYPT_HANDSHAKE_TASK_PRIO   1
static OSA_TASK_HANDLE_DEFINE(ncp_encrypt_handshake_thread);
static OSA_TASK_DEFINE(ncp_encrypt_handshake_task, NCP_HOST_ENCRYPT_HANDSHAKE_TASK_PRIO, 1, 4096, 0);

extern MCU_NCPCmd_DS_SYS_COMMAND *ncp_host_get_cmd_buffer_sys();

int port_mbedtls_send(void *ctx, const unsigned char *buf, size_t len)
{
    NCPCmd_DS_SYS_COMMAND *cmd = NULL;
    uint8_t *payload = NULL;
    int ret = 0;

    /* get the sem before send cmd and put the sem when recv cmd resp */
    (void) mcu_get_command_resp_sem();
    (void) mcu_get_command_lock();

    cmd = ncp_host_get_cmd_buffer_sys();
    cmd->header.cmd            = NCP_CMD_SYSTEM_CONFIG_ENCRYPT;
    cmd->header.size           = NCP_CMD_HEADER_LEN + sizeof(NCP_CMD_ENCRYPT) + len;
    cmd->header.result         = NCP_CMD_RESULT_OK;
    cmd->params.encrypt.action = NCP_CMD_ENCRYPT_ACTION_DATA;

    if (cmd->header.size > NCP_HOST_COMMAND_LEN)
    {
        ncp_e("cmd->header.size(%d) > NCP_HOST_COMMAND_LEN(%d)", 
                    cmd->header.size, NCP_HOST_COMMAND_LEN);
        return 0;
    }

    payload = (uint8_t*)cmd + NCP_CMD_HEADER_LEN + sizeof(NCP_CMD_ENCRYPT);
    (void) memcpy(payload, buf, len);

    ret = ncp_host_send_tlv_command();
    if (ret != 0)
    {
        ncp_e("**** port tx err %d", ret);
        return 0;
    }

    return len;
}

static int ncp_encrypt_verify(void)
{
    NCPCmd_DS_SYS_COMMAND *cmd = NULL;
    int ret = 0;

    /* get the sem before send cmd and put the sem when recv cmd resp */
    (void) mcu_get_command_resp_sem();
    (void) mcu_get_command_lock();

    cmd = ncp_host_get_cmd_buffer_sys();
    cmd->header.cmd            = NCP_CMD_SYSTEM_CONFIG_ENCRYPT;
    cmd->header.size           = NCP_CMD_HEADER_LEN + sizeof(NCP_CMD_ENCRYPT);
    cmd->header.result         = NCP_CMD_RESULT_OK;
    cmd->params.encrypt.action = NCP_CMD_ENCRYPT_ACTION_VERIFY;

    cmd->params.encrypt.arg = _verify_num;
    ncp_d("ncp encrypt num send is %d 0x%08X\r\n", _verify_num, _verify_num);

    ret = ncp_host_send_tlv_command();
    if (ret != 0)
    {
        ncp_e("**** ncp_host_send_tlv_command err %d", ret);
        return -NCP_FAIL;
    }

    return NCP_SUCCESS;
}

static void ncp_encrypt_handshake_task(void *pvParameters)
{
    int ret = 0;

    ncp_d("**** encrypt init task called\r\n");

    if (CRYPTO_InitHardware() != 0)
    {
        ncp_e("CRYPTO_InitHardware fail");
        return;
    }
    
    ret = ncp_encrypt_setup(NCP_TLS_ROLE_CLIENT);
    if (ret != TLS_OK)
    {
        ncp_e("ncp_encrypt_setup fail %d", ret);
        goto exit;
    }
   
    ncp_d("**** next call mbedtls_ssl_handshake\r\n");
    ret = mbedtls_ssl_handshake(&_mbedtls->ssl);
    if (ret != 0)
    {
        ncp_e("mbedtls mbedtls_ssl_handshake fail %d", ret);
        ncp_e("%s", mbedtls_high_level_strerr(ret));
        goto exit;
    }
    ncp_d("**** mbedtls_ssl_handshake succ\r\n");


    ret = ncp_tlv_adapter_encrypt_enable();
    if (ret != 0)
    {
        ncp_e("ncp_tlv_adapter_encrypt_enable err %d", ret);
    }
    else
    {
        ncp_d("\r\nncp encrypt communication is started\r\n\r\n");

        ret = ncp_encrypt_verify();
        if (ret != NCP_SUCCESS)
        {
            ncp_e("ncp_encrypt_verify error %d", ret);
        }
    }

exit:
    (void) ncp_encrypt_teardown();
    ncp_d("**** exit with %d", ret);
    (void) OSA_TaskDestroy(ncp_encrypt_handshake_thread);
}

int ncp_process_encrypt_event(uint8_t *res)
{
    NCP_ASSERT(res);
    MCU_NCPCmd_DS_SYS_COMMAND *event = (MCU_NCPCmd_DS_SYS_COMMAND *)res;
    uint8_t *evt_data = (uint8_t*)event + NCP_CMD_HEADER_LEN;
    uint16_t evt_data_len = event->header.size - NCP_CMD_HEADER_LEN;

    return ncp_encrypt_process_handshake_data(evt_data, evt_data_len);
}

int ncp_process_encrypt_stop_event(uint8_t *res)
{
    NCP_ASSERT(res);
    
    (void) PRINTF("NCP stop encrypted communication\r\n");
    ncp_tlv_adapter_encrypt_deinit();

    return NCP_SUCCESS;
}

int ncp_process_encrypt_response(uint8_t *res)
{
    MCU_NCPCmd_DS_SYS_COMMAND *cmd_res = (MCU_NCPCmd_DS_SYS_COMMAND *)res;
    uint16_t result                    = cmd_res->header.result;

    if (result != NCP_CMD_RESULT_OK)
    {
        ncp_e("NCP device encrypt cmd resp result error %d, action %d\r\n", 
            result, cmd_res->params.encrypt.action);
        return -NCP_FAIL;
    }

    if (cmd_res->params.encrypt.action == NCP_CMD_ENCRYPT_ACTION_INIT)
    {
        if (!_mbedtls)
        {
            (void)OSA_TaskCreate((osa_task_handle_t)ncp_encrypt_handshake_thread, 
                    OSA_TASK(ncp_encrypt_handshake_task), (osa_task_param_t)NULL);
        }
    }
    else if (cmd_res->params.encrypt.action == NCP_CMD_ENCRYPT_ACTION_DATA)
    {
        // handshake of mbedtls

    }
    else if (cmd_res->params.encrypt.action == NCP_CMD_ENCRYPT_ACTION_VERIFY)
    {
        uint32_t num_recv = cmd_res->params.encrypt.arg;
        ncp_d("NCP encrypt verify %s\r\n", _verify_num == num_recv ? "succ" : "fail");
        (void) PRINTF("NCP encryption verify succ\r\n");
    }
    else if (cmd_res->params.encrypt.action == NCP_CMD_ENCRYPT_ACTION_STOP)
    {
        (void) PRINTF("NCP stop encrypted communication resp is recved\r\n");
    }
    else
    {
        ncp_e("ncp encrypt cmd resp unkonwn action %d", cmd_res->params.encrypt.action);
    }

    return NCP_SUCCESS;
}

int ncp_encrypt_command(int argc, char **argv)
{
    NCPCmd_DS_SYS_COMMAND *cmd = NULL;
    ncp_d("**** start encrypt init\r\n");

    if (_mbedtls)
    {
        ncp_w("tls handshake is ongoing");
        return -NCP_FAIL;
    }

    if (ncp_tlv_adapter_is_encrypt_mode())
    {
        ncp_w("already is encrypted mode");
        return NCP_SUCCESS;
    }

    cmd = ncp_host_get_cmd_buffer_sys();
    cmd->header.cmd             = NCP_CMD_SYSTEM_CONFIG_ENCRYPT;
    cmd->header.size            = NCP_CMD_HEADER_LEN + sizeof(NCP_CMD_ENCRYPT);
    cmd->header.result          = NCP_CMD_RESULT_OK;
    cmd->params.encrypt.action  = NCP_CMD_ENCRYPT_ACTION_INIT;

    return NCP_SUCCESS;
}

int ncp_dbg_encrypt_stop_command(int argc, char **argv)
{
    NCPCmd_DS_SYS_COMMAND *cmd = NULL;
    ncp_d("**** stop encrypt communication\r\n");

    if (_mbedtls)
    {
        ncp_w("tls handshake is ongoing");
        return -NCP_FAIL;
    }

    if (!ncp_tlv_adapter_is_encrypt_mode())
    {
        ncp_w("current is NOT encrypted mode");
        return NCP_SUCCESS;
    }

    cmd = ncp_host_get_cmd_buffer_sys();
    cmd->header.cmd             = NCP_CMD_SYSTEM_CONFIG_ENCRYPT;
    cmd->header.size            = NCP_CMD_HEADER_LEN + sizeof(NCP_CMD_ENCRYPT);
    cmd->header.result          = NCP_CMD_RESULT_OK;
    cmd->params.encrypt.action  = NCP_CMD_ENCRYPT_ACTION_STOP;

    return NCP_SUCCESS;
}

int ncp_trigger_encrypted_communication(void)
{
    int ret = 0;
    /* get the sem before send cmd and put the sem when recv cmd resp */
    (void) mcu_get_command_resp_sem();
    ret = ncp_encrypt_command(0, NULL);
    if (ret != NCP_SUCCESS)
    {
        return -NCP_FAIL;
    }
    ret = ncp_host_send_tlv_command();
    if (ret != NCP_SUCCESS)
    {
        return -NCP_FAIL;
    }
    return NCP_SUCCESS;
}

#endif /* CONFIG_NCP_USE_ENCRYPT */
