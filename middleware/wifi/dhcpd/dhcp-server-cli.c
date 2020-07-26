/** @file dhcp-server-cli.c
 *
 *  @brief This file provides CLI based APIs for the DHCP Server
 *
 *  Copyright 2008-2020 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */

/** dhcp-server-main.c: CLI based APIs for the DHCP Server
 */
#include <string.h>

#include <wm_os.h>
#include <wm_net.h>
#include <dhcp-server.h>

#include "dhcp-priv.h"

/*
 * Command-Line Interface
 */

#define WAIT_FOR_UAP_START 5
extern uint32_t dhcp_address_timeout;
extern struct dhcp_server_data dhcps;
void dhcp_stat(int argc, char **argv)
{
    int i = 0;
    PRINTF("DHCP Server Lease Duration : %d seconds\r\n", (int)dhcp_address_timeout);
    if (dhcps.count_clients == 0)
    {
        PRINTF("No IP-MAC mapping stored\r\n");
    }
    else
    {
        PRINTF("Client IP\tClient MAC\r\n");
        for (i = 0; i < dhcps.count_clients && i < MAC_IP_CACHE_SIZE; i++)
        {
            PRINTF("%s\t%02X:%02X:%02X:%02X:%02X:%02X\r\n", inet_ntoa(dhcps.ip_mac_mapping[i].client_ip),
                   dhcps.ip_mac_mapping[i].client_mac[0], dhcps.ip_mac_mapping[i].client_mac[1],
                   dhcps.ip_mac_mapping[i].client_mac[2], dhcps.ip_mac_mapping[i].client_mac[3],
                   dhcps.ip_mac_mapping[i].client_mac[4], dhcps.ip_mac_mapping[i].client_mac[5]);
        }
    }
}
