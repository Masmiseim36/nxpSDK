/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/*
 * Copyright 2023-2024 NXP
 */

#include <stdio.h>

#include "osal.h"
#include "soem_netc_ep.h"
#include "fsl_phy.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"

//----------------------------------------------------------------------------//
int netc_ep_init(void *pri, uint8_t *macAddr)
{
	status_t result = kStatus_Success;
    struct netc_ep_if_port *port = pri;
    volatile uint32_t count = 30;
    bool link               = false;

	result = EP_Init(&port->g_ep_handle, &macAddr[0], port->ep_config, port->bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Wait PHY link up */
    EC_PRINT("Wait for PHY link up...\r\n");

    do
    {
		PHY_GetLinkStatus(&port->phy_handle, &link);
        count--;
    } while (count && !link);

	if (!link) {
		EC_PRINT( "PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
		return -1;
	}
	EP_MsixSetEntryMask(&port->g_ep_handle, port->bdrConfig->txBdrConfig[0].msixEntryIdx, false);
    EP_MsixSetEntryMask(&port->g_ep_handle, port->bdrConfig->rxBdrConfig[0].msixEntryIdx, false);

    return 0;
}

int netc_ep_send(void *pri, const void *payload, uint32_t len)
{
    struct netc_ep_if_port *port = pri;
	memcpy(port->g_txFrame, payload, len);
    netc_buffer_struct_t txBuff      = {.buffer = port->g_txFrame, .length = len};
    netc_frame_struct_t txFrame      = {.buffArray = &txBuff, .length = 1};
	port->txOver = false;
    if ( EP_SendFrame(&port->g_ep_handle, 0, &txFrame, NULL, NULL) != kStatus_Success) {
        return 0;
	}
 
	while (!port->txOver) { }
    EP_ReclaimTxDescriptor(&port->g_ep_handle, 0);
	return len;
}

int netc_ep_recv(void *pri, void *buf, uint32_t len)
{
    struct netc_ep_if_port *port = pri;
	uint32_t length;
	status_t result = kStatus_Success;
	result = EP_GetRxFrameSize(&port->g_ep_handle, 0, &length);

	if (result == kStatus_NETC_RxFrameEmpty || result != kStatus_Success) {
		return 0;
	}

	if (length != 0) {
		length = len;
		result = EP_ReceiveFrameCopy(&port->g_ep_handle, 0, buf, length, NULL);
		if (result != kStatus_Success)
			return 0;
		return length;
	}
	return 0;
}

int netc_ep_link_status(void *pri)
{
    bool link;
    struct netc_ep_if_port *port = pri;
    if (kStatus_Success == PHY_GetLinkStatus(&port->phy_handle, &link))
    {
        if (link)
            return 1;
        else
            return 0;
    }
    return -1;
}

int netc_ep_close(void *pri)
{
    return 0;
}
