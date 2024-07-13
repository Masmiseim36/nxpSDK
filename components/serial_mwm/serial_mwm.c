/*
 * Copyright 2020-2021, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "serial_mwm.h"
#include "serial_mwm_port.h"
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

/*******************************************************************************
 * Variables
 ******************************************************************************/
static char s_buff[MWM_BUFFER_SIZE];

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
     * #define BOARD_SERIAL_MWM_RST_WRITE(output) GPIO_PinWrite(BOARD_SERIAL_MWM_RST_GPIO, BOARD_SERIAL_MWM_RST_PIN,
     * output)
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
                s_buff[idx++] = '\\';
                s_buff[idx++] = c;
            }
            else
            {
                s_buff[idx++] = c;
            }
        }
        else
        {
            /* End of data mode: last two chars are $$ */
            s_buff[idx++] = '$';
        }

        if ((idx + 2u) >= MWM_BUFFER_SIZE)
        {
            ret = mwm_tx((uint8_t *)s_buff, idx);
            if (ret < 0)
            {
                return ret;
            }

            idx = 0u;
        }
    }

    if (idx > 0u)
    {
        ret = mwm_tx((uint8_t *)s_buff, idx);
        if (ret < 0)
        {
            return ret;
        }
    }

    return 0;
}

static uint32_t uint_to_str(const uint32_t n, char *str)
{
    uint32_t x = 10u;
    uint32_t i = 8u;
    while ((x < n) && (i > 0u))
    {
        x *= 10u;
        i--;
    }

    if (x > n)
    {
        x = x / 10u;
    }

    i          = 0u;
    uint32_t y = n;
    uint32_t z = 0u;

    while (x > 0u)
    {
        z        = y / x;
        str[i++] = ((uint8_t)z + '0');
        y -= z * x;
        x /= 10u;
    }

    str[i] = '\0';

    return i;
}

static int read_string_max(char *s, uint32_t max_len)
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

        s[idx++] = (char)c;

    } while ((c != 0u) && ((uint32_t)idx < max_len));

    if ((c != 0u) && ((uint32_t)idx == max_len))
    {
        return -1;
    }

    return idx;
}

static int read_int(int *n)
{
    int ret;

    ret = read_string_max(s_buff, MWM_BUFFER_SIZE);
    if (ret < 0)
    {
        return -1;
    }

    errno = 0;
    /* Parse int */
    *n = strtol(s_buff, NULL, 10);
    if (errno != 0)
    {
        return -1;
    }

    return 0;
}

static int read_uint(uint32_t *n)
{
    int ret;

    ret = read_string_max(s_buff, MWM_BUFFER_SIZE);
    if (ret < 0)
    {
        return -1;
    }

    errno = 0;
    /* Parse uint32_t */
    *n = strtoul(s_buff, NULL, 10);
    if (errno != 0)
    {
        return -1;
    }

    return 0;
}

static int read_string(const char *s)
{
    int ret;
    size_t n;

    n   = strlen(s);
    ret = mwm_rx((uint8_t *)s_buff, n);
    if (ret < 0)
    {
        return ret;
    }

    if (strncmp(s_buff, s, n) != 0)
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

    ret = mwm_rx((uint8_t *)s_buff, 2);
    if (ret < 0)
    {
        return ret;
    }

    /* Check values */
    if ((s_buff[0] != '\0') && (s_buff[1] != '\0'))
    {
        return -1;
    }

    return 0;
}

static int read_errno(int *socket_errno)
{
    int ret;
    ret = read_string("errno:");
    if (ret < 0)
    {
        return ret;
    }

    ret = read_int(socket_errno);
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

static int read_whole_response(char *buff, uint32_t max_len, uint32_t *resp_len)
{
    int ret;
    uint32_t idx = 0u;
    uint8_t c;
    bool end_loop = false;

    while (!end_loop)
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
                end_loop = true;
            }
            else
            {
                if (buff[idx - 1u] == '\0')
                {
                    end_loop = true;
                }
            }
        }

        buff[idx++] = (char)c;

        if ((idx == max_len) && !end_loop)
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

static int find_value(const char *key, const char **value, const char *buff, uint32_t buff_len)
{
    if ((key == NULL) || (value == NULL) || (buff == NULL) || (buff_len == 0u))
    {
        return -1;
    }

    uint32_t i       = 0u;
    uint32_t j       = 0u;
    uint32_t key_len = strlen(key);

    while (i < buff_len)
    {
        /* compare the key */
        while ((j < key_len) && (i < buff_len) && (buff[i] == key[j]))
        {
            j++;
            i++;
        }

        if (j == key_len)
        {
            *value = buff + i;
            return 1;
        }
        else
        {
            /* find next delimiter '\0' - begin of next key */
            while ((buff[i++] != '\0') && (i < buff_len))
            {
            }

            j = 0u;
        }
    }

    return 0;
}

/* Send cmd and return status code of received response */
static int mwm_cmd(char *cmd, uint32_t cmd_len)
{
    int ret;
    ret = mwm_tx((uint8_t *)cmd, cmd_len);
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
        int socket_errno = 1;
        ret              = read_errno(&socket_errno);
        if (ret < 0)
        {
            return ret;
        }

        return -socket_errno;
    }

    return status_code;
}

static uint32_t prepare_cmd(const char *cmd)
{
    s_buff[0] = 'm';
    s_buff[1] = 'w';
    s_buff[2] = 'm';
    s_buff[3] = '+';

    uint32_t i = 0u;
    char *dst  = &s_buff[4];
    while ((cmd[i] != '\0') && (i < (MWM_BUFFER_SIZE - 4u)))
    {
        *dst++ = cmd[i++];
    }

    return (i + 4u);
}

int mwm_version(char *value, uint32_t val_len)
{
    if ((value == NULL) || (val_len == 0u))
    {
        return -1;
    }

    uint32_t i = prepare_cmd("sver\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    ret = read_whole_response(value, val_len, NULL);

    return ret;
}

int mwm_upgrade(char *type, char *url)
{
    if (type == NULL)
    {
        return -1;
    }

    bool check_status = false;
    int state_code    = -1;

    const char cmd[] = "mwm+supg=";
    size_t i         = sizeof(cmd) - 1u;
    (void)memcpy(s_buff, cmd, i);

    if (strcmp(type, MWM_UPGRADE_STATUS) == 0)
    {
        s_buff[i++]  = '?';
        check_status = true;
    }
    else
    {
        if (url == NULL)
        {
            return -1;
        }

        size_t url_len = strlen(url);
        if ((url_len + i + 5u) > MWM_BUFFER_SIZE)
        {
            return -1;
        }

        const char *fw = NULL;
        if (strcmp(type, MWM_UPGRADE_MCU_FW) == 0)
        {
            fw = MWM_UPGRADE_MCU_FW;
        }
        else if (strcmp(type, MWM_UPGRADE_WLAN_FW) == 0)
        {
            fw = MWM_UPGRADE_WLAN_FW;
        }
        else
        {
            return -1;
        }

        s_buff[i++] = fw[0];
        s_buff[i++] = fw[1];
        s_buff[i++] = ',';
        (void)memcpy(&s_buff[i], url, url_len);
        i += url_len;
    }
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    uint32_t i = prepare_cmd("sreboot\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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

    uint32_t i = prepare_cmd("wver\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    ret = read_whole_response(value, val_len, NULL);

    return ret;
}

int mwm_wlan_start(void)
{
    uint32_t i = prepare_cmd("wstrt\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    uint32_t i = prepare_cmd("wstat\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
        const char *value = NULL;
        ret               = find_value("failure-reason-code:", &value, s_buff, resp_len);
        if (ret < 1)
        {
            return -1;
        }

        /* 1 - authentication failed */
        if (*value == '1')
        {
            return MWM_AUTH_FAILED;
        }

        /* 2 - network not found */
        if (*value == '2')
        {
            return MWM_NETWORK_NOT_FOUND;
        }
    }
    return state_code;
}

/* Reset to provisioning mode */
int mwm_wlan_prov(void)
{
    uint32_t i = prepare_cmd("wrprov\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    uint32_t i = prepare_cmd("winfo\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
        const char *value = NULL;
        ret               = find_value("ssid:", &value, s_buff, resp_len);
        if (ret < 1)
        {
            return -1;
        }
        (void)strcpy(ssid, value);

        ret = find_value("ip:", &value, s_buff, resp_len);
        if (ret < 1)
        {
            return -1;
        }
        (void)strcpy(ip_addr, value);
    }
    else
    {
        return -1;
    }

    return 0;
}

int mwm_wlan_scan(mwm_wlan_t *wlans, uint32_t wlan_count)
{
    uint32_t i = prepare_cmd("wscan\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    uint32_t scan_count = 0u;
    ret                 = read_uint(&scan_count);
    if (ret < 0)
    {
        return ret;
    }

    i = 0u;
    while (i < scan_count)
    {
        if (i < wlan_count)
        {
            wlans[i].bssid[17] = '\0';
            ret                = mwm_rx((uint8_t *)wlans[i].bssid, 17u);
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
            while ((j > 0u) && (s_buff[j] != '-'))
            {
                j--;
            }
            uint32_t rssi_idx = j;

            uint32_t cnt = 2;
            while ((j > 0u) && (cnt > 0u))
            {
                if (s_buff[j] == ',')
                {
                    cnt--;
                }
                j--;
            }

            uint32_t len = MIN(j, (sizeof(wlans[i].ssid) - 1u));
            if (len > 0u)
            {
                (void)memcpy((void *)wlans[i].ssid, (void *)(s_buff + 1), len);
            }
            wlans[i].ssid[len] = '\0';

            j += 2u;
            errno            = 0;
            wlans[i].channel = (uint8_t)strtoul(&s_buff[j], NULL, 10);
            if (errno != 0)
            {
                return -1;
            }

            wlans[i].rssi = (int8_t)strtol(&s_buff[rssi_idx], NULL, 10);
            if (errno != 0)
            {
                return -1;
            }

            j = rssi_idx;

            while (s_buff[j] != ',')
            {
                j++;
            }
            j++;

            wlans[i].security = (uint8_t)strtol(&s_buff[j], NULL, 10);
            if (errno != 0)
            {
                return -1;
            }
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

    return (int)MIN(scan_count, wlan_count);
}

int mwm_wlan_connect(void)
{
    uint32_t i = prepare_cmd("wcon\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    uint32_t i = prepare_cmd("wdcon\n");

    int ret;
    ret = mwm_cmd(s_buff, i);
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

    const char cmd[]  = "mwm+cget=";
    size_t i          = sizeof(cmd) - 1u;
    size_t module_len = strlen(module);
    size_t param_len  = strlen(param);
    if ((module_len + param_len + i + 3u) > MWM_BUFFER_SIZE)
    {
        return -1;
    }

    (void)memcpy(s_buff, cmd, i);
    (void)memcpy(&s_buff[i], module, module_len);
    i += module_len;
    s_buff[i++] = ',';
    (void)memcpy(&s_buff[i], param, param_len);
    i += param_len;
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    ret = read_whole_response(value, val_len, NULL);

    return ret;
}

int mwm_set_param(char *module, char *param, char *value)
{
    if ((module == NULL) || (param == NULL) || (value == NULL))
    {
        return -1;
    }

    const char cmd[]  = "mwm+cset=";
    size_t i          = sizeof(cmd) - 1u;
    size_t module_len = strlen(module);
    size_t param_len  = strlen(param);
    size_t value_len  = strlen(value);
    if ((module_len + param_len + value_len + i + 4u) > MWM_BUFFER_SIZE)
    {
        return -1;
    }

    (void)memcpy(s_buff, cmd, i);
    (void)memcpy(&s_buff[i], module, module_len);
    i += module_len;
    s_buff[i++] = ',';
    (void)memcpy(&s_buff[i], param, param_len);
    i += param_len;
    s_buff[i++] = ',';
    (void)memcpy(&s_buff[i], value, value_len);
    i += value_len;
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    ret = mwm_cmd(cmd, sizeof(cmd) - 1u);
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
    const char cmd[] = "mwm+ncon=";
    size_t i         = sizeof(cmd) - 1u;
    (void)memcpy(s_buff, cmd, i);
    i += uint_to_str((uint32_t)socket, &s_buff[i]);
    s_buff[i++]     = ',';
    size_t host_len = strlen(addr->host);
    (void)memcpy(&s_buff[i], addr->host, host_len);
    i += host_len;
    s_buff[i++] = ',';
    i += uint_to_str(addr->port, &s_buff[i]);
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_cmd(s_buff, i);
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}

int mwm_recv_timeout(int socket, void *buf, size_t len, uint32_t timeout_ms)
{
    const char cmd[] = "mwm+nrecv=";
    size_t i         = sizeof(cmd) - 1u;
    (void)memcpy(s_buff, cmd, i);
    i += uint_to_str((uint32_t)socket, &s_buff[i]);
    s_buff[i++] = ',';
    i += uint_to_str(len, &s_buff[i]);
    s_buff[i++] = ',';
    i += uint_to_str(timeout_ms, &s_buff[i]);
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_cmd(s_buff, i);
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

int mwm_recvfrom_timeout(int socket, void *buf, size_t len, uint32_t timeout_ms)
{
    const char cmd[] = "mwm+nrecvfrom=";
    size_t i         = sizeof(cmd) - 1u;
    (void)memcpy(s_buff, cmd, i);
    i += uint_to_str((uint32_t)socket, &s_buff[i]);
    s_buff[i++] = ',';
    i += uint_to_str(len, &s_buff[i]);
    s_buff[i++] = ',';
    i += uint_to_str(timeout_ms, &s_buff[i]);
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_cmd(s_buff, i);
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
    const char cmd[] = "mwm+nsend=";
    size_t i         = sizeof(cmd) - 1u;
    (void)memcpy(s_buff, cmd, i);
    i += uint_to_str((uint32_t)socket, &s_buff[i]);
    s_buff[i++] = ',';
    s_buff[i++] = '$';
    s_buff[i++] = '$';
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_tx((uint8_t *)s_buff, i);
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
        int socket_errno = 1;
        ret              = read_errno(&socket_errno);
        if (ret < 0)
        {
            return ret;
        }

        return -socket_errno;
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return (int)len;
}

int mwm_sendto(int socket, void *buf, int len, mwm_sockaddr_t *to, int tolen)
{
    const char cmd[] = "mwm+nsendto=";
    size_t i         = sizeof(cmd) - 1u;
    (void)memcpy(s_buff, cmd, i);
    i += uint_to_str((uint32_t)socket, &s_buff[i]);
    s_buff[i++]     = ',';
    size_t host_len = strlen(to->host);
    (void)memcpy(&s_buff[i], to->host, host_len);
    i += host_len;
    s_buff[i++] = ',';
    i += uint_to_str(to->port, &s_buff[i]);
    s_buff[i++] = ',';
    s_buff[i++] = '$';
    s_buff[i++] = '$';
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_tx((uint8_t *)s_buff, i);
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
        int socket_errno = 1;
        ret              = read_errno(&socket_errno);
        if (ret < 0)
        {
            return ret;
        }

        return -socket_errno;
    }

    /* Read end of response */
    ret = read_end();
    if (ret < 0)
    {
        return ret;
    }

    return (int)len;
}

int mwm_close(int socket)
{
    const char cmd[] = "mwm+nclose=";
    size_t i         = sizeof(cmd) - 1u;
    (void)memcpy(s_buff, cmd, i);
    i += uint_to_str((uint32_t)socket, &s_buff[i]);
    s_buff[i++] = '\n';

    int ret;
    ret = mwm_cmd(s_buff, i);
    if (ret < 0)
    {
        return ret;
    }

    /* Read end of response */
    ret = read_end();

    return ret;
}
