#include "assert.h"
#include "stdint.h"
#include "stdio.h"
#include "iperf_api.h"
#include <lwip/sockets.h>
#include "lwip/prot/dhcp.h"

#ifdef IPERF3_ENET

#include "enet_ethernetif.h"
#include "lwip/netifapi.h"
#include "board.h"
#include "lwip/tcpip.h"

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

/* IP address configuration. */
#define configIP_ADDR0 10
#define configIP_ADDR1 42
#define configIP_ADDR2 0
#define configIP_ADDR3 2

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 10
#define configGW_ADDR1 42
#define configGW_ADDR2 0
#define configGW_ADDR3 1


void setup_default_ip(struct netif *fsl_netif0,
                      ip4_addr_t *fsl_netif0_ipaddr,
                      ip4_addr_t *fsl_netif0_netmask,
                      ip4_addr_t *fsl_netif0_gw)
{
    err_t result = 0;

    IP4_ADDR(fsl_netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(fsl_netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(fsl_netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    result = netifapi_netif_set_addr(fsl_netif0, fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw);
    assert(0 == result);
}

void enet_init(void)
{
    err_t result = 0;
    static struct netif fsl_netif0;
    struct dhcp *pdhcp = NULL;
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = EXAMPLE_PHY_ADDRESS,
        .clockName  = EXAMPLE_CLOCK_NAME,
        .macAddress = configMAC_ADDR,
    };

    tcpip_init(NULL, NULL);

    result = netifapi_netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0,
                                ethernetif0_init, tcpip_input);
    assert(0 == result);

    result = netifapi_netif_set_default(&fsl_netif0);
    assert(0 == result);

    result = netifapi_netif_set_up(&fsl_netif0);
    assert(0 == result);

    dhcp_start(&fsl_netif0);
    pdhcp = (struct dhcp *)netif_get_client_data(&fsl_netif0, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    for (int i = 10; i > 0 && pdhcp->state != DHCP_STATE_BOUND; i--)
    {
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    if (pdhcp->state != DHCP_STATE_BOUND)
    {
        dhcp_stop(&fsl_netif0);
        setup_default_ip(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw);
    }
}
#endif

static struct sockaddr_in d_addr = {0};

struct sockaddr_in test = {0};

int iperf_ip_to_ip(struct iperf_ip *iperf_ip, uint32_t *ip)
{
    assert(!((NULL == iperf_ip) || (NULL == ip)));
    if ((NULL == iperf_ip) || (NULL == ip))
        return -1;

    *ip = ((iperf_ip->ip[0] & 0xFF) | ((iperf_ip->ip[1] & 0xFF) << 8) | ((iperf_ip->ip[2] & 0xFF) << 16) |
           ((iperf_ip->ip[3] & 0xFF) << 24));

    return 0;
}

void iperf_hw_init(struct iperf_ctx *ctx)
{ // alloc bufferu, setup ip,
    ctx->ctrl_buf = pvPortMalloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->ctrl_buf);
    memset(ctx->ctrl_buf, 0, IPERF_BUFFER_MAX);

    ctx->send_buf = pvPortMalloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->send_buf);
    memset(ctx->send_buf, 0, IPERF_BUFFER_MAX);

    ctx->recv_buf = pvPortMalloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->recv_buf);
    memset(ctx->recv_buf, 0, IPERF_BUFFER_MAX);

    iperf_ip_to_ip(&ctx->server_ip, &ctx->sock_ip);

    memset(&d_addr, 0, sizeof(struct sockaddr_in));
    d_addr.sin_family      = AF_INET;
    d_addr.sin_addr.s_addr = ctx->sock_ip;
    d_addr.sin_port        = htons(SENDER_PORT_NUM);
    d_addr.sin_len         = sizeof(d_addr);

    ctx->ctrl_addr     = &d_addr;
    ctx->ctrl_addr_len = sizeof(d_addr);
}

int iperf_socket(int protocol)
{
    int socket;

    if (protocol == 1)
        protocol = SOCK_STREAM;
    if (protocol == 2)
        protocol = SOCK_DGRAM;

    socket = lwip_socket(PF_INET, protocol, 0);
    assert(!(socket == -1));
    if ((socket == -1))
        return -1;
    /* Force non-blocking flag for UDP socket */
    if ((SOCK_DGRAM == protocol) && (lwip_fcntl(socket, F_SETFL, O_NONBLOCK) == -1))
        return -1;

    return socket;
}

int iperf_connect(int socket, void *addr, uint32_t addrlen)
{
    int conn;
    conn = lwip_connect(socket, (struct sockaddr *)&d_addr, sizeof(struct sockaddr_in));
    return conn;
}

int iperf_send(int socket, void *buffer, size_t len, int flag)
{
    int sent_bytes;
    sent_bytes = send(socket, buffer, len, flag);
    return sent_bytes;
}

int iperf_send_to(int socket, char *buffer, int len, int flags, void *to, int tolen)
{
    int sent_bytes;
    sent_bytes = lwip_sendto(socket, buffer, len, flags, (struct sockaddr *)to, tolen);
    return sent_bytes;
}

int iperf_recv_noblock(int socket, void *buf, size_t len, int flags)
{
    int recv_bytes = 0;
    int new_opt = 0, old_opt = 0;
    int ret;

    assert(NULL != buf);
    assert(-1 != socket);

    new_opt = old_opt = lwip_fcntl(socket, F_GETFL, 0);
    new_opt |= O_NONBLOCK;
    ret = lwip_fcntl(socket, F_SETFL, new_opt);
    LWIP_ASSERT("ret != -1", ret != -1);

    recv_bytes = lwip_recv(socket, buf, len, flags);

    ret = lwip_fcntl(socket, F_SETFL, old_opt);
    LWIP_ASSERT("ret != -1", ret != -1);

    return recv_bytes;
}

int iperf_recv_from(int socket, void *buf, size_t len, int flags)
{
    struct sockaddr_in tmp_struct = {0};
    u32_t sock_len                = sizeof(tmp_struct);
    int recv_bytes;

    recv_bytes = lwip_recvfrom(socket, buf, len, flags, (struct sockaddr *)&tmp_struct, &sock_len);

    return recv_bytes;
}

int iperf_recv_blocked(int socket, void *buf, size_t len, int flags)
{
    int recv_bytes;
    recv_bytes = lwip_recv(socket, buf, len, flags);
    return recv_bytes;
}

int iperf_socket_close(int socket)
{
    lwip_close(socket);
    return 0;
}
