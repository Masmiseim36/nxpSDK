/*
 * Copyright 2020-2021, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "serial_mwm.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MWM_BUFFER_SIZE     1024u
#define MWM_REBOOT_DELAY_MS 4000u

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern int mwm_port_init(void);
extern int mwm_port_deinit(void);
extern int mwm_tx(uint8_t *write_buf, uint32_t len);
extern int mwm_rx(uint8_t *read_buf, uint32_t len);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_buff[MWM_BUFFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
int mwm_init(void)
{
#if defined(BOARD_SERIAL_MWM_RST_WRITE)
    /* Reset Wi-Fi module */
    BOARD_SERIAL_MWM_RST_WRITE(0);
    vTaskDelay(pdMS_TO_TICKS(1));
    BOARD_SERIAL_MWM_RST_WRITE(1);
    vTaskDelay(pdMS_TO_TICKS(MWM_REBOOT_DELAY_MS));
#else
/*
 * Example of Wi-Fi reset pin settings:
 * #define BOARD_SERIAL_MWM_RST_GPIO          GPIO1
 * #define BOARD_SERIAL_MWM_RST_PIN           22
 * #define BOARD_SERIAL_MWM_RST_WRITE(output) GPIO_PinWrite(BOARD_SERIAL_MWM_RST_GPIO, BOARD_SERIAL_MWM_RST_PIN, output)
 */
#warning "Define BOARD_SERIAL_MWM_RST_WRITE(output) in board.h"
#endif

    return mwm_port_init();
}

static int mwm_tx_data(uint8_t *data, uint32_t len)
{
    int ret;
    char c;
    uint32_t idx = 0;
    uint32_t i;

    for (i = 0u; i < (len + 2u); i++)
    {
        if (i < len)
        {
            c = (char)data[i];
            if ((c == '\\') || (c == '$'))
            {
                s_buff[idx++] = (uint8_t)'\\';
                s_buff[idx++] = (uint8_t)c;
            }
            else
            {
                s_buff[idx++] = (uint8_t)c;
            }
        }
        else
        {
            /* End of data mode: last two chars are $$ */
            s_buff[idx++] = (uint8_t)'$';
        }

        if ((idx + 2u) >= MWM_BUFFER_SIZE)
        {
            ret = mwm_tx(s_buff, idx);
            if (ret < 0)
            {
                return ret;
            }

            idx = 0u;
        }
    }

    if (idx > 0u)
    {
        ret = mwm_tx(s_buff, idx);
        if (ret < 0)
        {
            return ret;
        }
    }

    return 0;
}

static int read_string_max(uint8_t *s, uint32_t max_len)
{
    int ret;
    int idx = 0;
    uint8_t c;

    do
    {
        ret = mwm_rx(&c, 1);
        if (ret != 0)
        {
            return -1;
        }

        s[idx++] = c;

    } while ((c != 0u) && ((uint32_t)idx < max_len));

    if ((c != 0u) && ((uint32_t)idx == max_len))
    {
        return -1;
    }

    return idx;
}

static int read_int(int *val)
{
    int ret;

    ret = read_string_max(s_buff, MWM_BUFFER_SIZE);
    if (ret < 0)
    {
        return -1;
    }

    /* Parse int */
    *val = strtol((char *)s_buff, NULL, 10);

    return 0;
}

static int read_string(const char *s)
{
    int ret;
    size_t n;

    n   = strlen(s);
    ret = mwm_rx(s_buff, n);
    if (ret < 0)
    {
        return ret;
    }

    if (strncmp((char *)s_buff, s, n) != 0)
    {
        return -1;
    }

    return 0;
}

/* Read 1 byte which should be equal to "\0" */
static int read_end(void)
{
    int ret;
    uint8_t c;

    ret = mwm_rx(&c, 1);
    if (ret < 0)
    {
        return ret;
    }

    /* Check value */
    if (c != 0u)
    {
        return -1;
    }

    return 0;
}

/* Read 2 bytes which should be equal to "\0\0" */
static int read_end2(void)
{
    int ret;

    ret = mwm_rx(s_buff, 2);
    if (ret < 0)
    {
        return ret;
    }

    /* Check values */
    if ((s_buff[0] != 0u) && (s_buff[1] != 0u))
    {
        return -1;
    }

    return 0;
}

static int read_errno(int *errno)
{
    int ret;
    ret = read_string("errno:");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_int(errno);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return 0;
}

static int read_whole_response(uint8_t *buff, uint32_t max_len, uint32_t *resp_len)
{
    int ret;
    uint32_t idx = 0u;
    uint8_t c;
    bool end = false;

    while (!end)
    {
        ret = mwm_rx(&c, 1);
        if (ret < 0)
        {
            return ret;
        }

        if (c == 0u)
        {
            if (idx == 0u)
            {
                end = true;
            }
            else
            {
                if (buff[idx - 1u] == 0u)
                {
                    end = true;
                }
            }
        }

        buff[idx++] = c;

        if ((idx == max_len) && !end)
        {
            return -1;
        }
    }

    if (resp_len != NULL)
    {
        *resp_len = idx;
    }

    return 0;
}

static int find_val(const char *key, char **val, uint8_t *buff, uint32_t buff_len)
{
    if ((key == NULL) || (val == NULL) || (buff == NULL) || (buff_len == 0u))
    {
        return -1;
    }

    uint32_t i       = 0u;
    uint32_t j       = 0u;
    uint32_t key_len = strlen(key);

    while (i < buff_len)
    {
        /* compare the key */
        while ((j < key_len) && (i < buff_len) && (buff[i] == (uint8_t)key[j]))
        {
            j++;
            i++;
        }

        if (j == key_len)
        {
            *val = (char *)(buff + i);
            return 1;
        }
        else
        {
            /* find next delimiter '\0' - begin of next key */
            while ((buff[i++] != 0u) && (i < buff_len))
            {
            }

            j = 0u;
        }
    }

    return 0;
}

/* Send cmd and return status code of received response */
static int mwm_cmd(char *cmd)
{
    int ret;
    size_t cmd_len;

    cmd_len = strlen(cmd);
    ret     = mwm_tx((uint8_t *)cmd, (uint32_t)cmd_len);
    if (ret < 0)
    {
        return -1;
    }

    int status_code = -1;
    ret             = read_int(&status_code);
    if (ret < 0)
    {
        return ret;
    }

    if (status_code == MWM_SOCKET_ERROR)
    {
        int errno = 1;
        ret       = read_errno(&errno);
        if (ret < 0)
        {
            return ret;
        }

        return -errno;
    }

    return status_code;
}

int mwm_version(char *value, uint32_t val_len)
{
    if ((value == NULL) || (val_len == 0u))
    {
        return -1;
    }

    int ret;
    ret = mwm_cmd("mwm+sver\n");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("version:");
    if (ret < 0)
    {
        return ret;
    }

    /* Read response */
    ret = read_whole_response((uint8_t *)value, val_len, NULL);

    return ret;
}

int mwm_upgrade(char *type, char *url)
{
    if (type == NULL)
    {
        return -1;
    }

    int ret;
    bool check_status = false;
    int state_code    = -1;

    if (strcmp(type, MWM_UPGRADE_STATUS) == 0)
    {
        check_status = true;
        ret          = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+supg=?\n");
    }
    else
    {
        if (url == NULL)
        {
            return -1;
        }

        ret = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+supg=%s,%s\n", type, url);
    }

    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_cmd((char *)s_buff);
    if (ret < 0)
    {
        return ret;
    }

    if (check_status == true)
    {
        ret = read_string("state-code:");
        if (ret < 0)
        {
            return ret;
        }

        /* Parse state-code value */
        ret = read_int(&state_code);
        if (ret < 0)
        {
            return ret;
        }
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    if (check_status == true)
    {
        return state_code;
    }

    return ret;
}

int mwm_reboot(void)
{
    int ret;
    ret = mwm_cmd("mwm+sreboot\n");
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    ret = mwm_port_deinit();
    if (ret < 0)
    {
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(MWM_REBOOT_DELAY_MS));
    ret = mwm_port_init();

    return ret;
}

int mwm_wlan_fw_version(char *value, uint32_t val_len)
{
    if ((value == NULL) || (val_len == 0u))
    {
        return -1;
    }

    int ret;
    ret = mwm_cmd("mwm+wver\n");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("wlan-version:");
    if (ret < 0)
    {
        return ret;
    }

    /* Read response */
    ret = read_whole_response((uint8_t *)value, val_len, NULL);

    return ret;
}

int mwm_wlan_start(void)
{
    int ret;
    ret = mwm_cmd("mwm+wstrt\n");
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}

int mwm_wlan_status(void)
{
    int ret;
    ret = mwm_cmd("mwm+wstat\n");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("state-code:");
    if (ret < 0)
    {
        return ret;
    }

    /* Parse state-code value */
    int state_code = -1;
    ret            = read_int(&state_code);
    if (ret < 0)
    {
        return ret;
    }

    /* Read response */
    uint32_t resp_len = 0;
    ret               = read_whole_response(s_buff, MWM_BUFFER_SIZE, &resp_len);
    if (ret < 0)
    {
        return ret;
    }

    /* Check for the connection error in response */
    if (resp_len > 1u)
    {
        char *val = NULL;
        ret       = find_val("failure-reason-code:", &val, s_buff, resp_len);
        if (ret < 1)
        {
            return -1;
        }

        /* 1 - authentication failed */
        if (*val == '1')
        {
            return MWM_AUTH_FAILED;
        }

        /* 2 - network not found */
        if (*val == '2')
        {
            return MWM_NETWORK_NOT_FOUND;
        }
    }
    return state_code;
}

/* Reset to provisioning mode */
int mwm_wlan_prov(void)
{
    int ret;
    ret = mwm_cmd("mwm+wrprov\n");
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}

int mwm_wlan_info(char *ssid, char *ip_addr)
{
    int ret;
    ret = mwm_cmd("mwm+winfo\n");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("state-code:");
    if (ret < 0)
    {
        return ret;
    }

    /* Parse state-code value */
    int state_code = -1;
    ret            = read_int(&state_code);
    if (ret < 0)
    {
        return ret;
    }

    /* Read response */
    uint32_t resp_len = 0;
    ret               = read_whole_response(s_buff, MWM_BUFFER_SIZE, &resp_len);
    if (ret < 0)
    {
        return ret;
    }

    if (resp_len > 1u)
    {
        char *val = NULL;
        ret       = find_val("ssid:", &val, s_buff, resp_len);
        if (ret < 1)
        {
            return -1;
        }
        (void)strcpy(ssid, val);

        ret = find_val("ip:", &val, s_buff, resp_len);
        if (ret < 1)
        {
            return -1;
        }
        (void)strcpy(ip_addr, val);
    }
    else
    {
        return -1;
    }

    return 0;
}

int mwm_wlan_scan(mwm_wlan_t *wlans, uint32_t wlan_count)
{
    int ret;
    ret = mwm_cmd("mwm+wscan\n");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("scan-count:");
    if (ret < 0)
    {
        return ret;
    }

    /* Parse scan-count value */
    int scan_count = -1;
    ret            = read_int(&scan_count);
    if (ret < 0)
    {
        return ret;
    }

    int i = 0;
    while (i < scan_count)
    {
        if (i < wlan_count)
        {
            wlans[i].bssid[17] = '\0';
            ret                = mwm_rx((uint8_t *)wlans[i].bssid, 17);
            if (ret < 0)
            {
                return ret;
            }

            ret = read_string_max(s_buff, MWM_BUFFER_SIZE);
            if (ret <= 0)
            {
                return ret;
            }

            uint32_t j = (uint32_t)ret - 1u;
            while ((j > 0u) && (s_buff[j] != (uint8_t)'-'))
            {
                j--;
            }
            uint32_t rssi_idx = j;

            uint32_t cnt = 2;
            while ((j > 0u) && (cnt > 0u))
            {
                if (s_buff[j] == (uint8_t)',')
                {
                    cnt--;
                }
                j--;
            }

            uint32_t len = MIN(j, (sizeof(wlans[i].ssid) - 1));
            if (len > 0u)
            {
                (void)memcpy((void *)wlans[i].ssid, (void *)(s_buff + 1), len);
            }
            wlans[i].ssid[len] = '\0';

            j += 2u;
            wlans[i].channel = (uint8_t)strtol((char *)&s_buff[j], NULL, 10);

            wlans[i].rssi = (int8_t)strtol((char *)&s_buff[rssi_idx], NULL, 10);

            j = rssi_idx;

            while (s_buff[j] != (uint8_t)',')
            {
                j++;
            }
            j++;

            wlans[i].security = (uint8_t)strtol((char *)&s_buff[j], NULL, 10);
        }
        else
        {
            ret = read_string_max(s_buff, MWM_BUFFER_SIZE);
            if (ret < 0)
            {
                return ret;
            }
        }

        i++;
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return MIN(scan_count, wlan_count);
}

int mwm_wlan_connect(void)
{
    int ret;
    ret = mwm_cmd("mwm+wcon\n");
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}

int mwm_wlan_disconnect(void)
{
    int ret;
    ret = mwm_cmd("mwm+wdcon\n");
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}

int mwm_get_param(char *module, char *param, char *value, uint32_t val_len)
{
    if ((module == NULL) || (param == NULL) || (value == NULL) || (val_len == 0u))
    {
        return -1;
    }

    int ret = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+cget=%s,%s\n", module, param);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_cmd((char *)s_buff);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string(param);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string(":");
    if (ret < 0)
    {
        return ret;
    }

    /* Read response */
    ret = read_whole_response((uint8_t *)value, val_len, NULL);

    return ret;
}

int mwm_set_param(char *module, char *param, char *value)
{
    if ((module == NULL) || (param == NULL) || (value == NULL))
    {
        return -1;
    }

    int ret = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+cset=%s,%s,%s\n", module, param, value);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_cmd((char *)s_buff);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return ret;
}

int mwm_socket(int type)
{
    if ((type != MWM_UDP) && (type != MWM_TCP))
    {
        return -1;
    }

    char cmd[] = "mwm+nsock=tcp\n";

    if (type == MWM_UDP)
    {
        cmd[10] = 'u';
        cmd[11] = 'd';
        cmd[12] = 'p';
    }

    int ret;
    ret = mwm_cmd(cmd);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("handle:");
    if (ret < 0)
    {
        return ret;
    }

    /* Parse handle value */
    int socket_handle = -1;
    ret               = read_int(&socket_handle);
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return socket_handle;
}

int mwm_connect(int socket, mwm_sockaddr_t *addr, uint32_t addrlen)
{
    int ret = -1;
    ret     = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+ncon=%d,%s,%d\n", socket, addr->host, addr->port);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_cmd((char *)s_buff);
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}

int mwm_recv_timeout(int socket, void *buf, size_t len, int timeout_ms)
{
    int ret = -1;
    ret     = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+nrecv=%d,%d,%d\n", socket, len, timeout_ms);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_cmd((char *)s_buff);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("content-len:");
    if (ret < 0)
    {
        return ret;
    }

    int content_len = 0;
    ret             = read_int(&content_len);
    if (ret < 0)
    {
        return ret;
    }

    /* Read data */
    ret = mwm_rx(buf, (uint32_t)content_len);
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end2();
    if (ret < 0)
    {
        return ret;
    }

    return content_len;
}

int mwm_recvfrom_timeout(int socket, void *buf, size_t len, int timeout_ms)
{
    int ret = -1;
    ret     = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+nrecvfrom=%d,%d,%d\n", socket, len, timeout_ms);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_cmd((char *)s_buff);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("content-len:");
    if (ret < 0)
    {
        return ret;
    }

    int content_len = 0;
    ret             = read_int(&content_len);
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string("status:received data from ");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_string_max(s_buff, MWM_BUFFER_SIZE);
    if (ret < 0)
    {
        return ret;
    }

    // ToDo - parse address of sender

    /* Read data */
    ret = mwm_rx(buf, (uint32_t)content_len);
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end2();
    if (ret < 0)
    {
        return ret;
    }

    return content_len;
}

int mwm_send(int socket, void *buf, size_t len)
{
    int ret = -1;
    ret     = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+nsend=%d,$$\n", socket);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_tx(s_buff, (uint32_t)ret);
    if (ret < 0)
    {
        return ret;
    }

    /* Send data */
    ret = mwm_tx_data(buf, len);
    if (ret < 0)
    {
        return ret;
    }

    int status_code = -1;
    ret             = read_int(&status_code);
    if (ret < 0)
    {
        return ret;
    }

    if (status_code == MWM_SOCKET_ERROR)
    {
        int errno = 1;
        ret       = read_errno(&errno);
        if (ret < 0)
        {
            return ret;
        }

        return -errno;
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return (int)len;
}

int mwm_sendto(int socket, char *buf, int len, mwm_sockaddr_t *to, int tolen)
{
    int ret = -1;
    ret     = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+nsendto=%d,%s,%d,$$\n", socket, to->host, to->port);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_tx(s_buff, (uint32_t)ret);
    if (ret < 0)
    {
        return ret;
    }

    /* Send data */
    ret = mwm_tx_data((uint8_t *)buf, (uint32_t)len);
    if (ret < 0)
    {
        return ret;
    }

    int status_code = -1;
    ret             = read_int(&status_code);
    if (ret < 0)
    {
        return ret;
    }

    if (status_code == MWM_SOCKET_ERROR)
    {
        int errno = 1;
        ret       = read_errno(&errno);
        if (ret < 0)
        {
            return ret;
        }

        return -errno;
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return (int)len;
    ;
}

int mwm_close(int socket)
{
    int ret = -1;
    ret     = snprintf((char *)s_buff, MWM_BUFFER_SIZE, "mwm+nclose=%d\n", socket);
    if ((ret <= 0) || ((uint32_t)ret > MWM_BUFFER_SIZE))
    {
        return -1;
    }

    ret = mwm_cmd((char *)s_buff);
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}
