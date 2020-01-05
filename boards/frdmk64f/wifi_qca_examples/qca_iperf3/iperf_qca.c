#include "assert.h"
#include "stdint.h"
#include "stdio.h"
#include "iperf_api.h"
#include "iperf_api.h"
#include "qcom_api.h"

extern QCA_CONTEXT_STRUCT *wlan_get_context(void);

WLAN_AUTH_MODE g_auth    = WLAN_AUTH_WPA2_PSK;
WLAN_CRYPT_TYPE g_cipher = WLAN_CRYPT_AES_CRYPT;

QCA_CONTEXT_STRUCT *enetCtx = NULL;

static SOCKADDR_T xTempAddress = {0};

int iperf_ip_to_qcom_ip(struct iperf_ip *iperf_ip, uint32_t *qcom_ip)
{
    assert(!((NULL == iperf_ip) || (NULL == qcom_ip)));
    if ((NULL == iperf_ip) || (NULL == qcom_ip))
        return -1;

    *qcom_ip = (((iperf_ip->ip[0]) & 0xFF) << 24) | (((iperf_ip->ip[1]) & 0xFF) << 16) |
               (((iperf_ip->ip[2]) & 0xFF) << 8) | (((iperf_ip->ip[3]) & 0xFF));

    return 0;
}

void iperf_hw_init(struct iperf_ctx *ctx)
{
    assert(NULL != ctx);

    enetCtx = wlan_get_context();
    assert(NULL != enetCtx);

    /* QCA specific allocator for send */
    ctx->ctrl_buf = custom_alloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->ctrl_buf);
    memset(ctx->ctrl_buf, 0, IPERF_BUFFER_MAX);

    /* QCA specific allocator for send */
    ctx->send_buf = custom_alloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->send_buf);
    memset(ctx->send_buf, 0, IPERF_BUFFER_MAX);

    ctx->recv_buf = custom_alloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->recv_buf);
    memset(ctx->recv_buf, 0, IPERF_BUFFER_MAX);
    /* Convert IP to QCA s_addr */
    iperf_ip_to_qcom_ip(&ctx->server_ip, &ctx->sock_ip);

    xTempAddress.sin_addr.s_addr = ctx->sock_ip;
    xTempAddress.sin_family      = ATH_AF_INET;
    xTempAddress.sin_port        = SENDER_PORT_NUM;

    ctx->ctrl_addr     = &xTempAddress;
    ctx->ctrl_addr_len = sizeof(xTempAddress);
}
int iperf_send(int socket, void *buffer, size_t len, int flag)
{
    int sent_bytes;
    sent_bytes = qcom_send(socket, buffer, len, flag);
    return sent_bytes;
}

int iperf_send_to(int socket, char *buffer, int len, int flags, void *to, int tolen)
{
    int sent_bytes;
    sent_bytes = qcom_sendto(socket, buffer, len, flags, (struct sockaddr *)to, tolen);
    return sent_bytes;
}

int iperf_connect(int socket, void *addr, uint32_t addrlen)
{
    int conn;
    conn = qcom_connect(socket, (struct sockaddr *)addr, addrlen);
    return conn;
}

int iperf_socket(int protocol)
{
    if (protocol == 1)
        protocol = SOCK_STREAM_TYPE;
    if (protocol == 2)
        protocol = SOCK_DGRAM_TYPE;
    int socket;
    socket = qcom_socket(ATH_AF_INET, protocol, 0);
    assert(socket != A_ERROR);
    return socket;
}

int iperf_recv_from(int socket, void *buf, size_t len, int flags)
{
    SOCKADDR_T tmp_struct = {0};
    uint16_t sock_len     = sizeof(tmp_struct);
    int recv_bytes;
    char *tmp_buf = NULL;

    recv_bytes = qcom_recvfrom(socket, &tmp_buf, len, flags, (struct sockaddr *)&tmp_struct, &sock_len);

    assert(((recv_bytes > 0) && (tmp_buf != NULL)) || ((recv_bytes <= 0) && (tmp_buf == NULL)));

    if ((recv_bytes > 0) && (tmp_buf != NULL))
    {
        memcpy(buf, tmp_buf, recv_bytes);
        zero_copy_free(tmp_buf);
    }

    return recv_bytes;
}

int iperf_recv_noblock(int socket, void *buf, size_t len, int flags)
{
    int recv_bytes;
    char *tmp_buf = NULL;
    assert(NULL != buf);
    assert(A_ERROR != socket);

    recv_bytes = qcom_recv(socket, &tmp_buf, len, flags);
    assert(((recv_bytes > 0) && (tmp_buf != NULL)) || ((recv_bytes <= 0) && (tmp_buf == NULL)));

    if ((recv_bytes > 0) && (tmp_buf != NULL))
    {
        memcpy(buf, tmp_buf, recv_bytes);
        zero_copy_free(tmp_buf);
    }

    return recv_bytes;
}

volatile int g_select_counter     = 0;
volatile int g_select_counter_max = 0;

int iperf_recv_blocked(int socket, void *buf, size_t len, int flags)
{
    int status = A_ERROR;
    assert(A_ERROR != socket);

    for (g_select_counter = 0; (g_select_counter < 20) && (status != 0); g_select_counter++)
    {
        status = t_select(enetCtx, socket, 10000);
    }

    if (g_select_counter_max < g_select_counter)
        g_select_counter_max = g_select_counter;

    assert(0 == status);

    if (status != 0)
    {
        return -1;
    }

    return iperf_recv_noblock(socket, buf, len, flags);
}

int iperf_socket_close(int socket)
{
    qcom_socket_close(socket);
    return 0;
}
