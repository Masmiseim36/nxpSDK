/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/*
 * Copyright 2023-2024 NXP
 */

#include <stdio.h>

#include "osal.h"
#include "soem_netc_swt.h"
#include "fsl_phy.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"

//----------------------------------------------------------------------------//
int netc_swt_init(void *pri, uint8_t *macAddr)
{
	status_t result = kStatus_Success;
    struct netc_swt_if_port *port = pri;
    volatile uint32_t count = 30;
    bool link               = false;

    result = EP_Init(&port->g_ep_handle, &macAddr[0], port->ep_config, port->bdrConfig);
    if (result != kStatus_Success)
    {
        EC_PRINT("Failed to EP_Init!\r\n");
        return result;
    }

	result = SWT_Init(&port->g_swt_handle, port->swt_config);
    if (result != kStatus_Success)
    {
        EC_PRINT("Failed to SWT_Init!\r\n");
        return result;
    }

    result = SWT_ManagementTxRxConfig(&port->g_swt_handle, &port->g_ep_handle, port->swtTxRxConfig);
    if (kStatus_Success != result)
    {
        EC_PRINT("Failed to config TxRx!\r\n");
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

int netc_swt_send(void *pri, const void *payload, uint32_t len)
{
    struct netc_swt_if_port *port = pri;
    swt_mgmt_tx_arg_t txArg = {0};
    txArg.ring = 0;
	memcpy(port->g_txFrame, payload, len);
    netc_buffer_struct_t txBuff      = {.buffer = port->g_txFrame, .length = len};
    netc_frame_struct_t txFrame      = {.buffArray = &txBuff, .length = 1};
	port->txOver = false;
    if ( SWT_SendFrame(&port->g_swt_handle, txArg, (netc_hw_port_idx_t) kNETC_SWITCH0Port0, false, &txFrame, NULL, NULL) != kStatus_Success) {
        return 0;
	}
 
	while (!port->txOver) { }
    SWT_ReclaimTxDescriptor(&port->g_swt_handle, false, 0);

	return len;
}

int netc_swt_recv(void *pri, void *buf, uint32_t len)
{
    struct netc_swt_if_port *port = pri;
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

int netc_swt_link_status(void *pri)
{
    bool link;
    struct netc_swt_if_port *port = pri;
    if (kStatus_Success == PHY_GetLinkStatus(&port->phy_handle, &link))
    {
        if (link)
            return 1;
        else
            return 0;
    }
    return -1;
}

int netc_swt_close(void *pri)
{
    return 0;
}
