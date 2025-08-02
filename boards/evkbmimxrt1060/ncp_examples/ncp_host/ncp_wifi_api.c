/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ncp_host_command_wifi.h"
#include "ncp_tlv_adapter.h"
#include "ncp_wifi_api.h"

/** Create a g_scan_result_info at ncp host side to save scan result, so that matter can get it when it wants*/
NCP_CMD_SCAN_NETWORK_INFO g_scan_result_info;
OSA_MUTEX_HANDLE_DEFINE(scan_lock);

int mcu_create_mutex_scan_lock()
{
    int ret;
    ret = OSA_MutexCreate((osa_mutex_handle_t)scan_lock);
    return ret;
}

int mcu_get_scan_lock()
{
    return OSA_MutexLock((osa_mutex_handle_t)scan_lock, osaWaitForever_c);
}

int mcu_put_scan_lock()
{
    return OSA_MutexUnlock(scan_lock);
}

static void wlan_ncp_get_conn_state_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_COMMAND * cmd_res = (NCPCmd_DS_COMMAND*) res;
    uint8_t sta_conn_stat = 0;

    NCP_CMD_CONNECT_STAT * conn = &cmd_res->params.conn_stat;
    sta_conn_stat               = conn->sta_conn_stat;

    memcpy(cmd_node->resp_buf, &sta_conn_stat, sizeof(uint8_t));
}

static void wlan_ncp_remove_network_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    int8_t remove_state = 0;
    NCPCmd_DS_COMMAND * cmd_res = (NCPCmd_DS_COMMAND*) res;
    NCP_CMD_NETWORK_REMOVE * network_remove_resp = &cmd_res->params.network_remove;
    remove_state                                 = network_remove_resp->remove_state;

    memcpy(cmd_node->resp_buf, &remove_state, sizeof(int8_t));
}

static void wlan_ncp_get_cmd_result_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCP_COMMAND *cmd_res = (NCP_COMMAND*)res;
    memcpy(cmd_node->resp_buf, &cmd_res->result, sizeof(uint16_t));
}

static void wlan_ncp_get_scan_result_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_COMMAND * cmd_res = (NCPCmd_DS_COMMAND*) res;
    memcpy(cmd_node->resp_buf, cmd_res, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_SCAN_NETWORK_INFO));
}

static void wlan_ncp_get_current_network_cb(void *res, ncp_cmd_node_t * cmd_node)
{  
    NCPCmd_DS_COMMAND * cmd_res       = (NCPCmd_DS_COMMAND*) res;
    ncp_current_network * sta_network = (ncp_current_network *)cmd_node->resp_buf;
    
    sta_network->result = cmd_res->header.result;
    (void) memcpy(&sta_network->sta_network, &cmd_res->params.current_network, sizeof(NCP_CMD_GET_CURRENT_NETWORK));
}

static void wlan_ncp_get_mac_address_cb(void *res, ncp_cmd_node_t * cmd_node)
{  
    NCPCmd_DS_COMMAND * cmd_res       = (NCPCmd_DS_COMMAND*) res;
    ncp_get_mac_addr * get_mac_addr = (ncp_get_mac_addr *)cmd_node->resp_buf;
    
    get_mac_addr->result = cmd_res->header.result;
    (void) memcpy(&get_mac_addr->mac_addr, &cmd_res->params.get_mac_addr, sizeof(NCP_CMD_GET_MAC_ADDRESS));
}

static void wlan_ncp_get_pkt_stats_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_COMMAND * cmd_res       = (NCPCmd_DS_COMMAND*) res;
    ncp_pkt_stats * get_pkt_stats = (ncp_pkt_stats *)cmd_node->resp_buf;
    
    get_pkt_stats->result = cmd_res->header.result;
    (void) memcpy(&get_pkt_stats->pkt_stats, &cmd_res->params.get_pkt_stats, sizeof(NCP_CMD_PKT_STATS));
}

static void wlan_ncp_get_current_rssi_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    short rssi = 0;
    NCPCmd_DS_COMMAND * cmd_res       = (NCPCmd_DS_COMMAND*) res;
    NCP_CMD_GET_CURRENT_RSSI *current_rssi = (NCP_CMD_GET_CURRENT_RSSI *)&cmd_res->params.current_rssi;
    rssi = current_rssi->rssi;

    (void) memcpy(cmd_node->resp_buf, &rssi, sizeof(short));
}

static void wlan_ncp_get_current_channel_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    uint8_t channel = 0;
    NCPCmd_DS_COMMAND * cmd_res       = (NCPCmd_DS_COMMAND*) res;
    NCP_CMD_GET_CURRENT_CHANNEL *current_channel = (NCP_CMD_GET_CURRENT_CHANNEL *)&cmd_res->params.current_channel;
    channel = current_channel->channel;

    (void) memcpy(cmd_node->resp_buf, &channel, sizeof(uint8_t));
}

static void wlan_ncp_get_ip_config_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_COMMAND * cmd_res       = (NCPCmd_DS_COMMAND*) res;
    NCP_CMD_IP_CONFIG *ip_config = (NCP_CMD_IP_CONFIG *)cmd_node->resp_buf;

    (void) memcpy(ip_config, &cmd_res->params.ip_config, sizeof(NCP_CMD_IP_CONFIG));
}

char * wlan_ncp_get_state(void)
{
    uint8_t ret = 0;
    uint8_t * cmd_resp_buf = OSA_MemoryAllocate(sizeof(uint8_t));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return NULL;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(uint8_t));

    uint8_t sta_conn_stat;

    NCPCmd_DS_COMMAND * conn_stat_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(conn_stat_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return NULL;
    }
    (void) memset((uint8_t *) conn_stat_command, 0, sizeof(NCP_COMMAND));

    conn_stat_command->header.cmd    = NCP_CMD_WLAN_STA_CONNECT_STAT;
    conn_stat_command->header.size   = NCP_CMD_HEADER_LEN;
    conn_stat_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(conn_stat_command, cmd_resp_buf, wlan_ncp_get_conn_state_cb);
    
    if(ret == NCP_STATUS_SUCCESS)
    {
        sta_conn_stat = * cmd_resp_buf;
    }
    else
    {
        sta_conn_stat = NCP_STATUS_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(conn_stat_command);

    switch (sta_conn_stat)
    {
    case WLAN_DISCONNECTED:
        return "DISCONNECTED";
    case WLAN_SCANNING:
        return "SCANNING";
    case WLAN_ASSOCIATING:
        return "ASSOCIATING";
    case WLAN_ASSOCIATED:
        return "ASSOCIATED";
    case WLAN_AUTHENTICATED:
        return "AUTHENTICATED";
    case WLAN_CONNECTING:
        return "CONNECTING";
    case WLAN_CONNECTED:
        return "COMPLETED";
    default:
        return "UNKNOWN";
    }
}

bool wlan_ncp_remove_network(NCP_WLAN_NETWORK * network)
{
    int8_t remove_state = NCP_STATUS_ERROR;
    uint8_t ret = 0;

    int8_t * cmd_resp_buf = OSA_MemoryAllocate(sizeof(int8_t));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return false;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(int8_t));

    NCPCmd_DS_COMMAND * network_remove_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_NETWORK_REMOVE));
    if(network_remove_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return false;
    }
    (void) memset((uint8_t *) network_remove_command, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_NETWORK_REMOVE));
    
    network_remove_command->header.cmd    = NCP_CMD_WLAN_NETWORK_REMOVE;
    network_remove_command->header.size   = NCP_CMD_HEADER_LEN;
    network_remove_command->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_NETWORK_REMOVE * network_remove = (NCP_CMD_NETWORK_REMOVE *) &network_remove_command->params.network_remove;

    (void) memcpy(network_remove->name, network->name, strlen(network->name));
    network_remove->remove_state = WM_SUCCESS;

    network_remove_command->header.size += sizeof(NCP_CMD_NETWORK_REMOVE);

    ret = ncp_tlv_send_wait_resp(network_remove_command, cmd_resp_buf, wlan_ncp_remove_network_cb);

    if(ret == NCP_STATUS_SUCCESS)
    {
        remove_state = * cmd_resp_buf;
    }
    else
    {
        remove_state = NCP_STATUS_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(network_remove_command);
    if (remove_state == NCP_STATUS_SUCCESS)
    {
        return true;
    }
    return false;
}

bool wlan_ncp_add_network(char * ssid, char * key, int8_t mode, int8_t frequency, char * network_name, int8_t key_len)
{
    uint16_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;

    uint16_t * cmd_resp_buf   = OSA_MemoryAllocate(sizeof(uint16_t));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return false;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(uint16_t));


    NCPCmd_DS_COMMAND * network_add_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_NETWORK_ADD) + sizeof(SSID_ParamSet_t) + sizeof(Security_ParamSet_t) + sizeof(BSSRole_ParamSet_t));
    if(network_add_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return false;
    }

    (void) memset((uint8_t *) network_add_command, 0, (sizeof(NCP_COMMAND) + sizeof(NCP_CMD_NETWORK_ADD) + sizeof(SSID_ParamSet_t) + sizeof(Security_ParamSet_t) + sizeof(BSSRole_ParamSet_t)));

    NCP_CMD_NETWORK_ADD * network_add_tlv   = (NCP_CMD_NETWORK_ADD *) &network_add_command->params.network_add;
    uint8_t * ptlv_pos                      = network_add_tlv->tlv_buf;
    uint32_t tlv_buf_len                    = 0;
    SSID_ParamSet_t * ssid_tlv              = NULL;
    BSSRole_ParamSet_t * role_tlv           = NULL;
    Security_ParamSet_t * security_tlv = NULL;

    (void) memcpy(network_add_tlv->name, network_name, strlen(network_name) + 1);

    if (strlen(ssid) > 32)
    {
        PRINTF("Error: SSID is too long\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        OSA_MemoryFree(network_add_command);
        return false;
    }

    ssid_tlv = (SSID_ParamSet_t *)ptlv_pos;
    (void) memcpy(ssid_tlv->ssid, ssid, strlen(ssid) + 1);
    ssid_tlv->header.type = NCP_CMD_NETWORK_SSID_TLV;
    ssid_tlv->header.size = sizeof(ssid_tlv->ssid);
    ptlv_pos += sizeof(SSID_ParamSet_t);
    tlv_buf_len += sizeof(SSID_ParamSet_t);

    role_tlv = (BSSRole_ParamSet_t *) ptlv_pos;
    if (mode == 2)
    {
        role_tlv->role = WLAN_BSS_ROLE_UAP;
    }
    else
    {
        role_tlv->role = WLAN_BSS_ROLE_STA;
    }
    role_tlv->header.type = NCP_CMD_NETWORK_ROLE_TLV;
    role_tlv->header.size = sizeof(role_tlv->role);
    ptlv_pos += sizeof(BSSRole_ParamSet_t);
    tlv_buf_len += sizeof(BSSRole_ParamSet_t);

    if (key_len > 0)
    {
        security_tlv               = (Security_ParamSet_t *) ptlv_pos;
        security_tlv->type         = WLAN_SECURITY_WILDCARD;
        security_tlv->password_len = key_len;
        /* copy the PSK phrase */
        if (security_tlv->password_len < WLAN_PSK_MIN_LENGTH || security_tlv->password_len >= WLAN_PSK_MAX_LENGTH)
        {
            ncp_e("Error: Invalid passphrase length %lu (expected ASCII characters: 8..63)\r\n", key_len);
            OSA_MemoryFree(cmd_resp_buf);
            OSA_MemoryFree(network_add_command);
            return NCP_STATUS_ERROR;
        }
        else
        {
            strncpy(security_tlv->password, key, key_len);
        }
        security_tlv->header.type = NCP_CMD_NETWORK_SECURITY_TLV;
        security_tlv->header.size =
            sizeof(security_tlv->type) + sizeof(security_tlv->password_len) + security_tlv->password_len;
        ptlv_pos += NCP_TLV_HEADER_LEN + security_tlv->header.size;
        tlv_buf_len += NCP_TLV_HEADER_LEN + security_tlv->header.size;
    }

    network_add_tlv->tlv_buf_len = tlv_buf_len;

    network_add_command->header.cmd = NCP_CMD_WLAN_NETWORK_ADD;
    network_add_command->header.size =
        NCP_CMD_HEADER_LEN + sizeof(network_add_tlv->name) + sizeof(network_add_tlv->tlv_buf_len) + tlv_buf_len;
    network_add_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(network_add_command, cmd_resp_buf, wlan_ncp_get_cmd_result_cb);

    if(ret == NCP_STATUS_SUCCESS)
    {
        result = * cmd_resp_buf;
    }
    else
    {
        result = NCP_STATUS_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(network_add_command);
    if (result == NCP_STATUS_SUCCESS)
    {
        return true;
    }
    return false;
}


bool wlan_ncp_disconnect(void)
{
    uint16_t disconnect_state = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;
    uint16_t * cmd_resp_buf   = OSA_MemoryAllocate(sizeof(uint16_t));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return false;
    }
    (void) memset((uint16_t *) cmd_resp_buf, 0, sizeof(uint16_t));

    NCPCmd_DS_COMMAND * disconnect_command = (NCPCmd_DS_COMMAND *)OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(disconnect_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return false;
    }
    (void) memset((uint8_t *) disconnect_command, 0, sizeof(NCP_COMMAND));

    disconnect_command->header.cmd    = NCP_CMD_WLAN_STA_DISCONNECT;
    disconnect_command->header.size   = NCP_CMD_HEADER_LEN;
    disconnect_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(disconnect_command, cmd_resp_buf, wlan_ncp_get_cmd_result_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        disconnect_state = * cmd_resp_buf;
    }
    else
    {
        disconnect_state = NCP_CMD_RESULT_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(disconnect_command);

    if (disconnect_state != NCP_CMD_RESULT_OK)
    {
        return false;
    }

    return true;
}

bool wlan_ncp_scan(void)
{
    uint16_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;
    
    mcu_get_scan_lock();
    
    NCPCmd_DS_COMMAND * cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_SCAN_NETWORK_INFO));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        mcu_put_scan_lock();
        return false;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_SCAN_NETWORK_INFO));

    NCPCmd_DS_COMMAND * scan_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(scan_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        mcu_put_scan_lock();
        return false;
    }
    (void) memset((uint8_t *) scan_command, 0, sizeof(NCP_COMMAND));

    scan_command->header.cmd    = NCP_CMD_WLAN_STA_SCAN;
    scan_command->header.size   = NCP_CMD_HEADER_LEN;
    scan_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(scan_command, cmd_resp_buf, wlan_ncp_get_scan_result_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        result = cmd_resp_buf->header.result;
    }
    else
    {
        result = NCP_CMD_RESULT_ERROR;
    }

    result = cmd_resp_buf->header.result;

    if (result == NCP_CMD_RESULT_OK)
    {   
        // if scan success, update the g_scan_result_info
        NCP_CMD_SCAN_NETWORK_INFO * scan_result = (NCP_CMD_SCAN_NETWORK_INFO *) &cmd_resp_buf->params.scan_network_info;
        g_scan_result_info.res_cnt              = scan_result->res_cnt;
        (void) memset(g_scan_result_info.res, 0, sizeof(NCP_WLAN_SCAN_RESULT) * NCP_MAX_AP_ENTRIES);
        (void) memcpy(g_scan_result_info.res, scan_result->res, sizeof(NCP_WLAN_SCAN_RESULT) * scan_result->res_cnt);
    }

    mcu_put_scan_lock();

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(scan_command);

    if (result == NCP_CMD_RESULT_OK)
    {
        return true;
    }
    return false;
}

int wlan_ncp_get_scan_result(unsigned int index, NCP_WLAN_SCAN_RESULT * res)
{
    if (res == NULL)
    {
        return NCP_STATUS_ERROR;
    }

    mcu_get_scan_lock();
    *res = g_scan_result_info.res[index];
    mcu_put_scan_lock();

    PRINTF("%s\r\n", res->ssid);
    PRINTF("	 BSSID		   : %02X:%02X:%02X:%02X:%02X:%02X\r\n", (unsigned int)res->bssid[0],
               (unsigned int)res->bssid[1], (unsigned int)res->bssid[2],
               (unsigned int)res->bssid[3], (unsigned int)res->bssid[4],
               (unsigned int)res->bssid[5]);
    PRINTF("	 RSSI		   : %ddBm\r\n", -(int)res->rssi);
    PRINTF("	 Channel	   : %d\r\n", (int)res->channel);

    return NCP_STATUS_SUCCESS;
}

bool wlan_ncp_get_scan_result_count(uint8_t * count)
{
    if (count == NULL)
    {
        return false;
    }

    mcu_get_scan_lock();
    *count = g_scan_result_info.res_cnt;
    mcu_put_scan_lock();

    return true;
}

bool wlan_ncp_connect(char * network_name)
{
    uint16_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;
    
    uint16_t * cmd_resp_buf = OSA_MemoryAllocate(sizeof(uint16_t));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return false;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(uint16_t));

    NCPCmd_DS_COMMAND * connect_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_WLAN_CONN));
    if(connect_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return false;
    }
    (void) memset((uint8_t *) connect_command, 0, (sizeof(NCP_COMMAND) + sizeof(NCP_CMD_WLAN_CONN)));

    connect_command->header.cmd    = NCP_CMD_WLAN_STA_CONNECT;
    connect_command->header.size   = NCP_CMD_HEADER_LEN;
    connect_command->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_WLAN_CONN * conn = (NCP_CMD_WLAN_CONN *) &connect_command->params.wlan_connect;
    (void) memcpy(conn->name, network_name, strlen(network_name) + 1);
    connect_command->header.size += sizeof(NCP_CMD_WLAN_CONN);

    ret = ncp_tlv_send_wait_resp(connect_command, cmd_resp_buf, wlan_ncp_get_cmd_result_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        result = *cmd_resp_buf;
    }
    else
    {
        result = NCP_CMD_RESULT_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(connect_command);

    if (result != NCP_CMD_RESULT_OK)
    {
        return false;
    }
    return true;
}

int wlan_ncp_get_current_network(NCP_WLAN_NETWORK * net_work)
{
    uint8_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;

    ncp_current_network * cmd_resp_buf = OSA_MemoryAllocate(sizeof(ncp_current_network));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return result;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(ncp_current_network));

    NCPCmd_DS_COMMAND * get_current_networks_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(get_current_networks_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return result;
    }
    (void) memset((uint8_t *) get_current_networks_command, 0, sizeof(NCP_COMMAND));

    get_current_networks_command->header.cmd    = NCP_CMD_WLAN_GET_CURRENT_NETWORK;
    get_current_networks_command->header.size   = NCP_CMD_HEADER_LEN;
    get_current_networks_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(get_current_networks_command, cmd_resp_buf, wlan_ncp_get_current_network_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        result = cmd_resp_buf->result;
        (void) memcpy(net_work, &cmd_resp_buf->sta_network, sizeof(NCP_CMD_GET_CURRENT_NETWORK));
    }
    else
    {
        result = NCP_CMD_RESULT_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(get_current_networks_command);

    if (result != NCP_CMD_RESULT_OK)
    {
        return NCP_STATUS_ERROR;
    }

    return NCP_STATUS_SUCCESS;
}

bool wlan_ncp_remove_all_networks(void)
{
    uint16_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;

    uint16_t * cmd_resp_buf = OSA_MemoryAllocate(sizeof(uint16_t));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return false;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(uint16_t));

    NCPCmd_DS_COMMAND * remove_all_networks_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(remove_all_networks_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return false;
    }
    (void) memset((uint8_t *) remove_all_networks_command, 0, sizeof(NCP_COMMAND));

    remove_all_networks_command->header.cmd    = NCP_CMD_WLAN_NETWORKS_REMOVE_ALL;
    remove_all_networks_command->header.size   = NCP_CMD_HEADER_LEN;
    remove_all_networks_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(remove_all_networks_command, cmd_resp_buf, wlan_ncp_get_cmd_result_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        result = *cmd_resp_buf;
    }
    else
    {
        result = NCP_CMD_RESULT_ERROR;
    }
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(remove_all_networks_command);

    if (result != NCP_CMD_RESULT_OK)
    {
        return false;
    }

    return true;
}

int wlan_ncp_get_mac_address(uint8_t * dest)
{
    uint16_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;

    ncp_get_mac_addr * cmd_resp_buf = OSA_MemoryAllocate(sizeof(ncp_get_mac_addr));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return result;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(ncp_get_mac_addr));

    NCPCmd_DS_COMMAND * get_mac_addr_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(get_mac_addr_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return result;
    }
    (void) memset((uint8_t *) get_mac_addr_command, 0, sizeof(NCP_COMMAND));
        
    get_mac_addr_command->header.cmd    = NCP_CMD_WLAN_STA_GET_MAC;
    get_mac_addr_command->header.size   = NCP_CMD_HEADER_LEN;
    get_mac_addr_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(get_mac_addr_command, cmd_resp_buf, wlan_ncp_get_mac_address_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        result = cmd_resp_buf->result;
        (void) memcpy(dest, &cmd_resp_buf->mac_addr.sta_mac, sizeof(cmd_resp_buf->mac_addr.sta_mac));
    }
    else
    {
        result = NCP_CMD_RESULT_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(get_mac_addr_command);

    if (result != NCP_CMD_RESULT_OK)
    {
        return NCP_STATUS_ERROR;
    }

    return NCP_STATUS_SUCCESS;
}

int wlan_ncp_get_pkt_stats(NCP_CMD_PKT_STATS *stats)
{
    uint16_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;

    ncp_pkt_stats * cmd_resp_buf = OSA_MemoryAllocate(sizeof(ncp_pkt_stats));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return result;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(ncp_pkt_stats));

    NCPCmd_DS_COMMAND * get_pkt_stats_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(get_pkt_stats_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return result;
    }
    (void) memset((uint8_t *) get_pkt_stats_command, 0, sizeof(NCP_COMMAND));
        
    get_pkt_stats_command->header.cmd    = NCP_CMD_WLAN_GET_PKT_STATS;
    get_pkt_stats_command->header.size   = NCP_CMD_HEADER_LEN;
    get_pkt_stats_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(get_pkt_stats_command, cmd_resp_buf, wlan_ncp_get_pkt_stats_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        result = cmd_resp_buf->result;
        (void) memcpy(stats, &cmd_resp_buf->pkt_stats, sizeof(NCP_CMD_PKT_STATS));
    }
    else
    {
        result = NCP_CMD_RESULT_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(get_pkt_stats_command);

    if (result != NCP_CMD_RESULT_OK)
    {
        return NCP_STATUS_ERROR;
    }

    return NCP_STATUS_SUCCESS;
}

int wlan_ncp_get_current_rssi(short * rssi)
{
    uint8_t ret = 0;
    short * cmd_resp_buf = OSA_MemoryAllocate(sizeof(short));
    short rssi_bak;

    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return NCP_STATUS_ERROR;
    }
    (void) memset((short *) cmd_resp_buf, 0, sizeof(short));

    NCPCmd_DS_COMMAND * get_rssi_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(get_rssi_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return NCP_STATUS_ERROR;
    }
    (void) memset((uint8_t *) get_rssi_command, 0, sizeof(NCP_COMMAND));

    get_rssi_command->header.cmd    = NCP_CMD_WLAN_STA_GET_CURRENT_RSSI;
    get_rssi_command->header.size   = NCP_CMD_HEADER_LEN;
    get_rssi_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(get_rssi_command, cmd_resp_buf, wlan_ncp_get_current_rssi_cb);
    
    if(ret == NCP_STATUS_SUCCESS)
    {
        memcpy(rssi, cmd_resp_buf, sizeof(rssi_bak));
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(get_rssi_command);

    return ret;
}

uint8_t wlan_ncp_get_current_channel()
{
    uint8_t ret = 0;
    uint8_t current_channel;
    uint8_t * cmd_resp_buf = OSA_MemoryAllocate(sizeof(uint8_t));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return NCP_STATUS_ERROR;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(uint8_t));

    NCPCmd_DS_COMMAND * get_current_channel_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(get_current_channel_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return NCP_STATUS_ERROR;
    }
    (void) memset((uint8_t *) get_current_channel_command, 0, sizeof(NCP_COMMAND));

    get_current_channel_command->header.cmd    = NCP_CMD_WLAN_STA_GET_CURRENT_CHANNEL;
    get_current_channel_command->header.size   = NCP_CMD_HEADER_LEN;
    get_current_channel_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(get_current_channel_command, cmd_resp_buf, wlan_ncp_get_current_channel_cb);
    
    if(ret == NCP_STATUS_SUCCESS)
    {
        current_channel = *cmd_resp_buf;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(get_current_channel_command);

    return current_channel;
}

int wlan_ncp_get_ip_config(NCP_CMD_IP_CONFIG * ip_config)
{
    uint16_t result = NCP_STATUS_ERROR;
    uint16_t ret = NCP_STATUS_ERROR;

    NCP_CMD_IP_CONFIG * cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_IP_CONFIG));
    if(cmd_resp_buf == NULL)
    {
        PRINTF("failed to malloc cmd_resp_buf!\r\n");
        return result;
    }
    (void) memset((uint8_t *) cmd_resp_buf, 0, sizeof(NCP_CMD_IP_CONFIG));

    NCPCmd_DS_COMMAND * get_ip_config_command = OSA_MemoryAllocate(sizeof(NCP_COMMAND));
    if(get_ip_config_command == NULL)
    {
        PRINTF("failed to malloc cmd buff.\r\n");
        OSA_MemoryFree(cmd_resp_buf);
        return result;
    }
    (void) memset((uint8_t *) get_ip_config_command, 0, sizeof(NCP_COMMAND));

    get_ip_config_command->header.cmd    = NCP_CMD_WLAN_GET_IP_CONFIG;
    get_ip_config_command->header.size   = NCP_CMD_HEADER_LEN;
    get_ip_config_command->header.result = NCP_CMD_RESULT_OK;

    ret = ncp_tlv_send_wait_resp(get_ip_config_command, cmd_resp_buf, wlan_ncp_get_ip_config_cb);
    if(ret == NCP_STATUS_SUCCESS)
    {
        (void) memcpy(ip_config, cmd_resp_buf, sizeof(NCP_CMD_IP_CONFIG));
        result = NCP_CMD_RESULT_OK;
    }
    else
    {
        result = NCP_CMD_RESULT_ERROR;
    }

    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(get_ip_config_command);

    if (result != NCP_CMD_RESULT_OK)
    {
        return NCP_STATUS_ERROR;
    }

    return NCP_STATUS_SUCCESS;
}
