#include "wwd.h"
#include "wwd_wiced.h"
#include "wwd_network.h"
#include "wwd_constants.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
//#include "ethernetif.h"
#include "lwip/ip_addr.h"

#define AP_SSID_START           "imxrt_wifi"
#define AP_PASS                 "12345678"
#define AP_SEC                  WICED_SECURITY_WPA2_AES_PSK  /* WICED_SECURITY_OPEN */
#define AP_CHANNEL              (1)

/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 1

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 0
#define configNET_MASK3 0

static struct netif wiced_if;

void start_dhcp_server( uint32_t local_addr );
void quit_dhcp_server( void );

int test_apsta(int argc, char *argv[])
{
    wiced_ssid_t ap_ssid;
    //wiced_mac_t  my_mac;
    ip4_addr_t ap_ipaddr;
    ip4_addr_t ap_netmask;
    uint8_t    ap_channel = AP_CHANNEL;

    if(argc == 2)
    {
        ap_channel = (uint8_t) atoi(argv[1]);
    } 

    /* Create the SSID */
    strcpy((char*) ap_ssid.value, AP_SSID_START);
    ap_ssid.length = strlen( (char*)ap_ssid.value );
    PRINTF("Starting Access Point: SSID: %s, Chnl: %d\n", 
            (char*)ap_ssid.value, ap_channel);

    /* Start the access point */
    wwd_wifi_start_ap( &ap_ssid, AP_SEC, (uint8_t*) AP_PASS, sizeof( AP_PASS ) - 1, ap_channel );

    IP4_ADDR(&ap_ipaddr, configIP_ADDR0, configIP_ADDR1,
                                     configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&ap_netmask, configNET_MASK0, configNET_MASK1,
                                     configNET_MASK2, configNET_MASK3);
    if ( NULL == netif_add( &wiced_if, &ap_ipaddr, &ap_netmask, &ap_ipaddr, (void*) WWD_AP_INTERFACE, wlanif_init, tcpip_input) )
    {
        WPRINT_APP_ERROR( ( "Failed to start network interface\n" ) );
        return 0;
    }
    netif_set_default( &wiced_if );
    netif_set_up( &wiced_if );

    PRINTF( "Network ready IP: %u.%u.%u.%u\n", (unsigned char) ( ( htonl( wiced_if.ip_addr.addr ) >> 24 ) & 0xff ),
                                                      (unsigned char) ( ( htonl( wiced_if.ip_addr.addr ) >> 16 ) & 0xff ),
                                                      (unsigned char) ( ( htonl( wiced_if.ip_addr.addr ) >> 8 ) & 0xff ),
                                                      (unsigned char) ( ( htonl( wiced_if.ip_addr.addr ) >> 0 ) & 0xff ) ) ;

    /* Create DHCP servers */
    start_dhcp_server( ap_ipaddr.addr );
    return 0;
}

void test_stop_ap()
{
    wwd_wifi_deauth_all_associated_client_stas(WWD_DOT11_RC_UNSPECIFIED, 
                                               WWD_AP_INTERFACE);
    quit_dhcp_server();
    wwd_wifi_stop_ap();
    netif_remove(&wiced_if);
}


