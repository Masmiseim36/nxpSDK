/** @file wlan_basic_cli.c
 *
 *  @brief  This file provides Connection Manager CLI
 *
 *  Copyright 2008-2020 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#include <wlan.h>
#include <cli.h>

static void test_wfa_wlan_version(int argc, char **argv)
{
    unsigned char i;
    char version_str[MLAN_MAX_VER_STR_LEN];
    char *ext_ver_str = wlan_get_firmware_version_ext();
    (void)strcpy(version_str, ext_ver_str);
    for (i = 0; i < strlen(version_str); i++)
    {
        if (version_str[i] == '.' && version_str[i + 1U] == 'p')
        {
            version_str[i] = '\0';
            break;
        }
    }

    (void)PRINTF("WLAN Version : %s\r\n", version_str);
}

static void test_wlan_version(int argc, char **argv)
{
#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
    char *version_str;

    version_str = wlan_get_firmware_version_ext();

    (void)PRINTF("WLAN Driver Version   : %s\r\n", WLAN_DRV_VERSION);
    (void)PRINTF("WLAN Firmware Version : %s\r\n", version_str);
#endif
}

static void test_wlan_get_mac_address(int argc, char **argv)
{
    uint8_t mac[6];

    (void)PRINTF("MAC address\r\n");
    if (wlan_get_mac_address(mac) != 0)
    {
        (void)PRINTF("Error: unable to retrieve MAC address\r\n");
    }
    else
    {
        (void)PRINTF("%02X:%02X:%02X:%02X:%02X:%02X\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
}

static struct cli_command wlan_wfa_basic_commands[] = {
    {"wlan-version", NULL, test_wfa_wlan_version},
    {"wlan-mac", NULL, test_wlan_get_mac_address},
};

static struct cli_command wlan_basic_commands[] = {
    {"wlan-version", NULL, test_wlan_version},
    {"wlan-mac", NULL, test_wlan_get_mac_address},
};

static bool wlan_wfa_basic_cli_init_done = false;

int wlan_wfa_basic_cli_init(void)
{
    unsigned int i;

    for (i = 0; i < sizeof(wlan_wfa_basic_commands) / sizeof(struct cli_command); i++)
    {
        if (cli_register_command(&wlan_wfa_basic_commands[i]) != 0)
        {
            return WLAN_ERROR_ACTION;
        }
    }

    wlan_wfa_basic_cli_init_done = true;

    return WLAN_ERROR_NONE;
}

int wlan_basic_cli_init(void)
{
    unsigned int i;

    if (wlan_wfa_basic_cli_init_done)
    {
        return WLAN_ERROR_NONE;
    }

    for (i = 0; i < sizeof(wlan_basic_commands) / sizeof(struct cli_command); i++)
    {
        if (cli_register_command(&wlan_basic_commands[i]) != 0)
        {
            return WLAN_ERROR_ACTION;
        }
    }

    return WLAN_ERROR_NONE;
}
