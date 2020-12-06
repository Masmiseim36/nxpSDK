/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "network_connection.h"

#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"

#if USE_ETHERNET_CONNECTION
#include "fsl_phydp83848.h"
#include "fsl_enet_mdio.h"

#include "enet_ethernetif.h"
#elif USE_WIFI_CONNECTION
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "wwd.h"
#include "wwd_constants.h"
#include "wwd_network.h"
#include "wwd_wiced.h"

#include "dhcp_server.h"
#include "wifi_credentials.h"

#define AP_SECURITY WICED_SECURITY_WPA2_AES_PSK
#define AP_CHANNEL  (1)

__attribute__((weak)) void APP_Wifi_UX_Callback(network_wifi_conn_state_t state)
{
    configPRINTF(("Network connection state, %d\r\n", state));
}

#else
/* should never happen */
#endif

#if USE_ETHERNET_CONNECTION
/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x00, 0x04, 0x9F, 0x05, 0x1C, 0x81 \
    }

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* PHY operations. */
#define EXAMPLE_PHY_OPS phydp83848_ops

static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};
#endif /* USE_ETHERNET_CONNECTION */

static struct netif ip_netif;
static bool s_connectState                  = false;
static connection_update_fn *update_link_cb = NULL;

TickType_t get_mqtt_connection_backoff(uint32_t retry_attempts, uint32_t base_delay)
{
    /** Example
     *  base_delay = 500ms
     *  retry_attempts = 3
     *  exponential = 150ms = ((base_delay / 10) * retry attempts)
     *  delay_adder_from_base = 950ms = (base_delay + (exponential * retry_attempts)
     *  full_delay = 1450ms = base_delay + delay_adder_from_base
     */
    return (pdMS_TO_TICKS((base_delay) + ((base_delay + ((base_delay / 10) * retry_attempts) * retry_attempts))));
}

void APP_Connect_Update_Handler_Set(connection_update_fn *func)
{
    update_link_cb = func;
}

ip_addr_t get_ipaddress(void)
{
    return ip_netif.ip_addr;
}

bool get_connect_state(void)
{
    return s_connectState;
}

status_t APP_NETWORK_Re_Link(void)
{
    /* Wait indefinitely for the link to re-establish. */
    do
    {
        vTaskDelay(portTICK_PERIOD_MS * 1000);
    } while (!get_connect_state());

#if USE_WIFI_CONNECTION

    /**
     * For WiFi connection, link loss is associated with loss of RF signal.
     *
     * Best practice here is to re-establish RF connection once the WiFi
     * Access Point's RF signal is present.
     */

    vTaskDelay(portTICK_PERIOD_MS * 1000);

    configPRINTF(("Resetting WiFi connection...\r\n"));
    APP_NETWORK_Uninit();

    configPRINTF(("...connecting to Access Point...\r\n"));
    return APP_NETWORK_Wifi_Connect(true, false);
#else
    return kStatus_Success;
#endif
}

#if USE_ETHERNET_CONNECTION

void enet_status_callback(struct netif *netif)
{
    bool linkUp = false;

    if (netif_is_link_up(netif))
    {
        linkUp = true;
    }
    else
    {
        linkUp = false;
    }

    /* Only call if it's a valid function pointer */
    if (update_link_cb)
    {
        (update_link_cb)(linkUp);
    }

    s_connectState = linkUp;
}

void APP_NETWORK_Init(bool use_dhcp)
{
    ip4_addr_t ipaddr, netmask, gw, dnsserver;
    struct dhcp *dhcp;
    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
    };

    if (use_dhcp)
    {
        IP4_ADDR(&ipaddr, 0, 0, 0, 0);
        IP4_ADDR(&netmask, 0, 0, 0, 0);
        IP4_ADDR(&gw, 0, 0, 0, 0);
    }
    else
    {
        IP4_ADDR(&ipaddr, 10, 81, 17, 27);
        IP4_ADDR(&netmask, 255, 255, 252, 0);
        IP4_ADDR(&gw, 10, 81, 19, 254);

        IP4_ADDR(&dnsserver, 10, 81, 250, 10);
        dns_setserver(0, &dnsserver);
        IP4_ADDR(&dnsserver, 10, 80, 20, 250);
        dns_setserver(1, &dnsserver);
    }

    configPRINTF(("APP_NETWORK_Init: start\r\n"));
    vTaskDelay(100);

    tcpip_init(NULL, NULL);

    if(enet_config.phyHandle->mdioHandle->resource.csrClock_Hz == 0U)
    {
        enet_config.phyHandle->mdioHandle->resource.csrClock_Hz = CLOCK_GetPllFreq(kCLOCK_PllEnet);
    }

    netif_add(&ip_netif, &ipaddr, &netmask, &gw, &enet_config, ethernetif0_init, tcpip_input);
    netif_set_default(&ip_netif);
    netif_set_up(&ip_netif);
    netif_set_link_callback(&ip_netif, &enet_status_callback);

    if (use_dhcp)
    {
        configPRINTF(("Getting IP address from DHCP ...\r\n"));
        dhcp_start(&ip_netif);

        dhcp = (struct dhcp *)netif_get_client_data(&ip_netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
        while (dhcp->state != DHCP_STATE_BOUND)
            vTaskDelay(1000);

        if (dhcp->state == DHCP_STATE_BOUND)
        {
            configPRINTF(("IPv4 Address: %u.%u.%u.%u\r\n", ((u8_t *)&ip_netif.ip_addr.addr)[0],
                          ((u8_t *)&ip_netif.ip_addr.addr)[1], ((u8_t *)&ip_netif.ip_addr.addr)[2],
                          ((u8_t *)&ip_netif.ip_addr.addr)[3]));
        }

        configPRINTF(("DHCP OK\r\n"));
    }

    if (netif_is_link_up(&ip_netif))
    {
        s_connectState = true;
    }
    else
    {
        s_connectState = false;
    }
}

void APP_NETWORK_Uninit()
{
    /* do nothing */
}
#elif USE_WIFI_CONNECTION

extern wiced_result_t wiced_wlan_connectivity_init(void);

wiced_result_t register_link_events(wiced_security_t security, void *);

static wifi_cred_t s_wifi_cred    = {0};
static wiced_security_t auth_type = WICED_SECURITY_WPA2_MIXED_PSK;
static struct netif ap_netif;

static void wifi_network_leave()
{
    wwd_wifi_leave(WWD_STA_INTERFACE);
}

static wwd_result_t wlan_connectivity_init(void)
{
    wwd_result_t err = WWD_SUCCESS;

    tcpip_init(NULL, NULL);
    (void)host_rtos_delay_milliseconds((uint32_t)1000);

    /* Main function to initialize wifi platform */
    configPRINTF(("Initializing WiFi Connectivity ...\r\n"));
    if ((err = wiced_wlan_connectivity_init()) != WWD_SUCCESS)
    {
        configPRINTF(("Could not initialize WiFi Connectivity, error: %d \r\n", err));
        assert(err == WWD_SUCCESS);
    }

    return err;
}

void wifi_connect_update_handler(wiced_bool_t event)
{
    /* notify the app part about the event */
    if (update_link_cb)
    {
        update_link_cb((bool)event);
    }

    switch (event)
    {
        case WICED_FALSE:
            if ((bool)WICED_FALSE != s_connectState)
            {
                s_connectState = (bool)WICED_FALSE;
                netif_set_link_down(&ip_netif);
                configPRINTF(("WiFi Link lost!\r\n"));
            }
            break;
        case WICED_TRUE:
            if ((bool)WICED_TRUE != s_connectState)
            {
                s_connectState = (bool)WICED_TRUE;
                netif_set_link_up(&ip_netif);
                configPRINTF(("WiFi Link established...\r\n"));
            }
            break;
        default:
            break;
    }
}

static wwd_result_t wifi_join(void)
{
    wwd_result_t err = WWD_SUCCESS;

    (void)host_rtos_delay_milliseconds((uint32_t)1000);

    auth_type = s_wifi_cred.password.length ? WICED_SECURITY_WPA2_MIXED_PSK : WICED_SECURITY_OPEN;

    err = wwd_wifi_join(&s_wifi_cred.ssid, auth_type, s_wifi_cred.password.value, s_wifi_cred.password.length, NULL,
                        WWD_STA_INTERFACE);
    if (err != WWD_SUCCESS)
    {
        configPRINTF(("Failed to join %s, error: %d\r\n", s_wifi_cred.ssid.value, err));
    }
    else
    {
        configPRINTF(("Successfully joined %s\r\n", s_wifi_cred.ssid.value));
        if (update_link_cb)
        {
            register_link_events(auth_type, wifi_connect_update_handler);
        }
        s_connectState = (bool)WICED_TRUE;
    }

    (void)host_rtos_delay_milliseconds((uint32_t)1000);

    return err;
}

static wwd_result_t wifi_if_add(bool use_dhcp)
{
    ip4_addr_t ipaddr, netmask, gw;
    struct dhcp *dhcp;
    wwd_result_t err = WWD_SUCCESS;

    (void)host_rtos_delay_milliseconds((uint32_t)1000);

    if (use_dhcp)
    {
        IP4_ADDR(&ipaddr, 0, 0, 0, 0);
        IP4_ADDR(&netmask, 0, 0, 0, 0);
        IP4_ADDR(&gw, 0, 0, 0, 0);
    }
    else
    {
        IP4_ADDR(&ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
        IP4_ADDR(&netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
        IP4_ADDR(&gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);
    }

    netif_add(&ip_netif, &ipaddr, &netmask, &gw, (void *)WWD_STA_INTERFACE, wlanif_init, tcpip_input);
    netif_set_default(&ip_netif);
    netif_set_up(&ip_netif);

    if (use_dhcp)
    {
        configPRINTF(("Getting IP address from DHCP ...\r\n"));
        dhcp_start(&ip_netif);

        dhcp = (struct dhcp *)netif_get_client_data(&ip_netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
        while (dhcp->state != DHCP_STATE_BOUND)
        {
            vTaskDelay(1000);

            if ((bool)WICED_FALSE == s_connectState)
            {
                configPRINTF(("Link loss while waiting for DHCP bound, aborting ...\r\n"));
                err = WWD_WLAN_WLAN_DOWN;
                break;
            }
        }

        if (dhcp->state == DHCP_STATE_BOUND)
        {
            configPRINTF(("IPv4 Address: %u.%u.%u.%u\r\n", ((u8_t *)&ip_netif.ip_addr.addr)[0],
                          ((u8_t *)&ip_netif.ip_addr.addr)[1], ((u8_t *)&ip_netif.ip_addr.addr)[2],
                          ((u8_t *)&ip_netif.ip_addr.addr)[3]));

            configPRINTF(("DHCP OK\r\n"));
        }
    }

    return err;
}

void APP_NETWORK_Init(void)
{
    APP_Wifi_UX_Callback(NETWORK_WIFI_CONN_WIFI_SETUP);

    wlan_connectivity_init();
}

status_t APP_NETWORK_Wifi_Connect(bool use_dhcp, bool provideUxLed)
{
    wwd_result_t err = WWD_SUCCESS;

    /* try to get credentials from flash */
    status_t status = wifi_credentials_flash_get(&s_wifi_cred);
    assert(status == 0);

    /* no wifi credentials in flash? wait for them to be sent via usb cdc */
    if (check_valid_credentials(&s_wifi_cred))
    {
        configPRINTF(("Found no credentials in flash\r\n"));
        return WIFI_CONNECT_NO_CRED;
    }
    else
    {
        configPRINTF(("Found credentials in flash, joining wifi network\r\n"));
    }

    while (1)
    {
        uint32_t attempts     = 0;
        uint32_t authAttempts = 0;

        for (;;)
        {
            configPRINTF(("Attempt %d to connect to wifi network %s ...\r\n", attempts + 1, s_wifi_cred.ssid.value));
            err = wifi_join();

            if (err == WWD_SUCCESS)
            {
                break;
            }
            else if ((err == WWD_NETWORK_NOT_FOUND) || /* The specified SSID isn't reachable (password provided) */
                     (err == WWD_INVALID_JOIN_STATUS)) /* The specified SSID isn't reachable (no password provided) */
            {
                // Wait for network availably if the SSID isn't reachable
                if (attempts > 2)
                {
                    if (provideUxLed)
                    {
                        // Being to alert the user that something is wrong
                        APP_Wifi_UX_Callback(NETWORK_WIFI_CONN_NO_ACCESS_POINT);
                    }

                    // Give user some time to enter new credentials
                    vTaskDelay(portTICK_PERIOD_MS * 15000);

                    /* get credentials from flash */
                    status = wifi_credentials_flash_get(&s_wifi_cred);
                    assert(status == 0);
                }
            }
            else if (err == WWD_NOT_AUTHENTICATED)
            {
                authAttempts++;

                if (authAttempts > 3)
                {
                    // We have tried enough to authenticate, our setup is incorrect
                    break;
                }
            }
            else
            {
                if (attempts > 1)
                {
                    // Something is definitely wrong... ABORT!
                    break;
                }
            }

            attempts++;
        }

        if (err != WWD_SUCCESS)
        {
            configPRINTF(("Joining network failed\r\n"));
            if ((err == WWD_NOT_AUTHENTICATED) ||         /* No password provided (0 length) */
                (err == WWD_INVALID_KEY) ||               /* Invalid password provided */
                (err == WWD_EAPOL_KEY_PACKET_M3_TIMEOUT)) /* Incorrect WPA2/WPA password */
            {
                return WIFI_CONNECT_WRONG_CRED;
            }
            else
            {
                return WIFI_CONNECT_FAILED;
            }
        }
        else
        {
            if (provideUxLed)
            {
                APP_Wifi_UX_Callback(NETWORK_WIFI_CONN_ACCESS_POINT_FOUND);
            }
            break;
        }
    }

    err = wifi_if_add(use_dhcp);
    return err;
}

void APP_NETWORK_Uninit()
{
    wifi_network_leave();
    netif_remove(&ip_netif);
    /* We removed the interface from netif_list, it is safe to fill the struct with 0 */
    memset(&ip_netif, 0, sizeof(ip_netif));
    s_connectState = (bool)WICED_FALSE;
}

status_t APP_NETWORK_Wifi_StartAP(wiced_ssid_t ap_ssid, char *ap_passwd)
{
    wwd_result_t err = WWD_SUCCESS;
    ip4_addr_t ap_ipaddr;
    ip4_addr_t ap_netmask;
    ip4_addr_t ap_gw;

    if ((ap_ssid.length == 0) || (ap_passwd == NULL))
    {
        configPRINTF(("Access Point cannot be started, wrong credentials\r\n"));
        return 1;
    }

    /* Start the Access Point */
    configPRINTF(
        ("Access Point Start: SSID '%.*s', Password '%s'\r\n", ap_ssid.length, (char *)ap_ssid.value, ap_passwd));

    err = wwd_wifi_start_ap(&ap_ssid, AP_SECURITY, (uint8_t *)ap_passwd, strlen(ap_passwd), AP_CHANNEL);

    if (err != WWD_SUCCESS)
    {
        configPRINTF(("Failed to start the AP, error: %d\r\n", err));
        return 1;
    }

    IP4_ADDR(&ap_ipaddr, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);
    IP4_ADDR(&ap_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&ap_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    if (NULL ==
        netif_add(&ap_netif, &ap_ipaddr, &ap_netmask, &ap_gw, (void *)WWD_AP_INTERFACE, wlanif_init, tcpip_input))
    {
        configPRINTF(("Failed to add the AP\n"));
        return 1;
    }
    netif_set_default(&ap_netif);
    netif_set_up(&ap_netif);

    configPRINTF(("AP Network ready, IPv4 Address: %u.%u.%u.%u\r\n", ((u8_t *)&ap_netif.ip_addr.addr)[0],
                  ((u8_t *)&ap_netif.ip_addr.addr)[1], ((u8_t *)&ap_netif.ip_addr.addr)[2],
                  ((u8_t *)&ap_netif.ip_addr.addr)[3]));

    /* Create DHCP server */
    start_dhcp_server(ap_ipaddr.addr);

    return 0;
}

status_t APP_NETWORK_Wifi_StopAP(void)
{
    wwd_result_t err = WWD_SUCCESS;
    status_t status  = 0;

    err = wwd_wifi_deauth_all_associated_client_stas(WWD_DOT11_RC_UNSPECIFIED, WWD_AP_INTERFACE);
    if (err != WWD_SUCCESS)
    {
        configPRINTF(("Failed to deauthenticate all client STAs associated, error: %d\r\n", err));
        status = 1;
    }

    quit_dhcp_server();

    err = wwd_wifi_stop_ap();
    if (err != WWD_SUCCESS)
    {
        configPRINTF(("Failed to stop the AP, error: %d\r\n", err));
        status = 1;
    }

    netif_remove(&ap_netif);

    return status;
}

status_t APP_NETWORK_Wifi_CheckCred(void)
{
    wifi_cred_t wifi_cred = {0};

    /* try to get credentials from flash */
    if (wifi_credentials_flash_get(&wifi_cred))
    {
        return 1;
    }

    if (check_valid_credentials(&wifi_cred))
    {
        return 1;
    }

    return 0;
}

#endif /* USE_WIFI_CONNECTION */
