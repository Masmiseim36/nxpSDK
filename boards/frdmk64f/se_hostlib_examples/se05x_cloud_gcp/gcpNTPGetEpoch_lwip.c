/**
 * @file gcpNTPGetEpoch.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2017,2018,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * Get network EPOCH for timestamp
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#include "gcpJWTDemo.h"

#if AX_EMBEDDED

#include "fsl_device_registers.h"
#if defined(LPC_ENET)
#include "lwip/api.h"
#include "lwip/ip.h"
#include "lwip/netdb.h"
#elif defined(LPC_WIFI)
#include "serial_mwm.h"
#endif
#include "httpsclient.h"
#include "sm_demo_utils.h"
#include "nxLog_App.h"

/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */

#define NTP_SERVER_NAME "time.google.com"
#define NTP_SERVER_PORT "123"
#define NTP_SERVER_PORT_INT 123
/* Used to avoid warnings in case of unused parameters in function pointers */
#define IOT_UNUSED(x) (void)(x)
/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

typedef __PACKED_STRUCT
{
    uint8_t LI_VN_Mode; // LI = leap indicator, VN = version number
    uint8_t Stratum;
    uint8_t PollInterval;
    uint8_t Precision;
    uint32_t Root_Delay;
    uint32_t Root_Dispersion;
    uint32_t Reference_Identifier;
    //Reference Timestamp;
    uint32_t RefTs_Seconds;
    uint32_t RefTs_Fraction;
    //Originate Timestamp
    uint32_t OrgntTs_Seconds;
    uint32_t OrgntTs_Fraction;
    //Receive Timestamp
    uint32_t RcvRs_Seconds;
    uint32_t RcvRs_Fraction;
    //Transmit Timestamp
    uint32_t TrnsTs_Seconds;
    uint32_t TrnsTs_Fraction;
}
NTP_CommandResponse_t;

union NTP_Buffer {
    NTP_CommandResponse_t cmd_rsp;
    uint8_t buffer[sizeof(NTP_CommandResponse_t)];
};
#if LPC_WIFI
#define CSV_BYTES_TO_UINT32_IPADDR(a0, a1, a2, a3) \
    (((uint32_t)(a0)&0xFF) << 24) | (((uint32_t)(a1)&0xFF) << 16) | (((uint32_t)(a2)&0xFF) << 8) | ((uint32_t)(a3)&0xFF)
#define UINT32_IPADDR_TO_CSV_BYTES(a) \
    ((uint8_t)((a) >> 24) & 0xFF), (uint8_t)(((a) >> 16) & 0xFF), (uint8_t)(((a) >> 8) & 0xFF), (uint8_t)((a)&0xFF)
extern uint8_t Wifi_IP[4];
#endif

#define MIN_EPOCH ((2018 - 1970) * 365 * 24 * 60 * 60)
#define MAX_EPOCH ((2038 - 1970) * 365 * 24 * 60 * 60)

#define NTP_leap 0 /* Not interested in leap seconds */
#define NTP_version 4
#define NTP_mode 3 /* client */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
#if defined(LPC_ENET)
static int lwipRecvTimeout(void *fd, unsigned char *buf, size_t len, uint32_t timeout_ms);
static unsigned long gcpNTPGetEpochEnet(void);
#elif defined(LPC_WIFI)
static int _traceQcomApi = 0;
static unsigned long gcpNTPGetEpochWifi(void);

int isValueFailed(int32_t value, int32_t failValue, const char *funcName)
{
    if (value == failValue) {
        LOG_E("ERROR: %s() returned %d\r\n", funcName, value);
    }
    else if (_traceQcomApi) {
        LOG_I("%s() OK\r\n", funcName);
    }
    return (value == failValue);
}
#ifndef ntohl
uint32_t ntohl(uint32_t input)
{
    uint8_t buffer[4] = {0};
    memcpy(&buffer, &input, sizeof(buffer));
    return ((uint32_t)buffer[3] << 0 * 8) | ((uint32_t)buffer[2] << 1 * 8) | ((uint32_t)buffer[1] << 2 * 8) |
           ((uint32_t)buffer[0] << 3 * 8);
}
#endif
#define HTONS(n) (((((uint32_t)(n)&0xFF)) << 8) | (((uint32_t)(n)&0xFF00) >> 8))
#endif
/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
unsigned long gcpNTPGetEpoch()
{
#if defined(LPC_ENET)
    return gcpNTPGetEpochEnet();
#elif defined(LPC_WIFI)
    return gcpNTPGetEpochWifi();
#endif
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */
#if defined(LPC_ENET)
static int lwipRecvTimeout(void *fd, unsigned char *buf, size_t len, uint32_t timeout_ms)
{
    int fd_socket = *((int *)fd);
    int ret = 0;
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd_socket, &read_set);
    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    ret = lwip_select(fd_socket + 1, &read_set, NULL, NULL, &timeout);
    if (ret < 0) {
        return ret;
    }
    else if (ret == 0) {
        LOG_W("lwip_select returned 0\r\n");
        return -1;
    }
    ret = lwip_recv(fd_socket, buf, len, 0);
    return ret;
}
unsigned long gcpNTPGetEpochEnet()
{
    int nwstatus = NETWORK_ERR_NET_SOCKET_FAILED;
    int fd = -1;
    int ret;
    unsigned long RefTs_Seconds;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_UDP;

    ret = getaddrinfo(NTP_SERVER_NAME, NTP_SERVER_PORT, &hints, &res);

    if ((ret != 0) || (res == NULL)) {
        nwstatus = NETWORK_ERR_NET_UNKNOWN_HOST;
        goto exit;
    }

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd < 0) {
        nwstatus = NETWORK_ERR_NET_SOCKET_FAILED;
        goto exit;
    }

    ret = connect(fd, res->ai_addr, res->ai_addrlen);

    freeaddrinfo(res);
    if (ret != 0) {
        close(fd);
        nwstatus = NETWORK_ERR_NET_CONNECT_FAILED;
        goto exit;
    }

    union NTP_Buffer ntp_buffer = {0};
    ntp_buffer.cmd_rsp.LI_VN_Mode = (NTP_leap << 6 | NTP_version << 3 | NTP_mode);

    ret = lwip_send(fd, ntp_buffer.buffer, sizeof(ntp_buffer), 0);
    if (ret <= 0) {
        LOG_W(" failed\n  ! lwip_send returned %d\n\n", ret);
        nwstatus = NETWORK_ERR_NET_SOCKET_FAILED;
        goto exit;
    }

    ret = lwipRecvTimeout(&fd, ntp_buffer.buffer, sizeof(ntp_buffer), 2000);

    closesocket(fd);
    if (ret <= 0) {
        LOG_W("failed\n  ! read returned %d\n\n", ret);
        nwstatus = NETWORK_ERR_NET_SOCKET_FAILED;
        goto exit;
    }

    RefTs_Seconds = (ntohl(ntp_buffer.cmd_rsp.RefTs_Seconds));
    RefTs_Seconds -= (((70ul * 365ul) + 17ul) * 24ul * 60ul * 60ul);
    closesocket(fd);

    if (RefTs_Seconds < MIN_EPOCH) {
        goto exit;
    }
    if (RefTs_Seconds > MAX_EPOCH) {
        goto exit;
    }
    LOG_I("Current EPOCH = %d", RefTs_Seconds);
    return RefTs_Seconds;

exit:
    LOG_W("ERROR Getting EPOCH %d\r\n", nwstatus);
    IOT_UNUSED(nwstatus);
    close(fd);
    return 0;
}
#elif defined(LPC_WIFI)
int wlan_get_state()
{
    int ret = mwm_wlan_status();
    if (ret < 0)
    {
        LOG_E("Failed to get WLAN status, error: %d", ret);
    }

    return ret;
}

unsigned long gcpNTPGetEpochWifi(void)
{
    int s = 0;
    int ret = 0;
    unsigned long RefTs_Seconds;
    mwm_sockaddr_t http_srv_addr = {.host = NTP_SERVER_NAME, .port = NTP_SERVER_PORT_INT};
    union NTP_Buffer ntp_buffer = {0};

    ret = wlan_get_state();
    if (ret != MWM_CONNECTED)
    {
        LOG_E("WLAN must be in connected state");
        return 0;
    }

    s = mwm_socket(MWM_UDP);
    if (s < 0)
    {
        LOG_E("Could not create socket");
        goto exit;
    }

    ret = mwm_connect(s, &http_srv_addr, sizeof(http_srv_addr));
    if (ret != 0)
    {
        LOG_E("Could not connect to server, error: %d", ret);
        goto exit;
    }

    ntp_buffer.cmd_rsp.LI_VN_Mode = 0x1b; //(NTP_leap << 6 | NTP_version << 3 | NTP_mode);

    /* Send HTTP GET request */
    ret      = mwm_send(s, (void *) &ntp_buffer.buffer[0], sizeof(ntp_buffer));
    if (ret <= 0)
    {
        LOG_E("Could not send data, error: %d", ret);
        goto exit;
    }

    ret      = mwm_recv_timeout(s, (void *) &ntp_buffer.buffer[0], sizeof(ntp_buffer), 0);
    if (ret <= 0)
    {
        LOG_E("Could not receive data, error: %d", ret);
        goto exit;
    }

    ret = mwm_close(s);
    if (ret < 0)
    {
        LOG_E("Could not close socket, error: %d", ret);
        return 0;
    }

    RefTs_Seconds = (ntohl(ntp_buffer.cmd_rsp.RefTs_Seconds));
    RefTs_Seconds -= (((70ul * 365ul) + 17ul) * 24ul * 60ul * 60ul);

    if (RefTs_Seconds < MIN_EPOCH) {
        goto exit;
    }
    if (RefTs_Seconds > MAX_EPOCH) {
        goto exit;
    }
    LOG_I("Current EPOCH = %ld", RefTs_Seconds);
    return RefTs_Seconds;

exit:
    LOG_E("ERROR Getting EPOCH %d", ret);

    ret = mwm_close(s);
    if (ret < 0)
    {
        LOG_E("Could not close socket, error: %d", ret);
    }
    return 0;
}
#endif

#endif //#if AX_EMBEDDED
