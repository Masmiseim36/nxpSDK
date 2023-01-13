// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2022 NXP
 */

#include <stdio.h>

#include "osal.h"
#include "enet.h"
#include "fsl_enet.h"
#include "fsl_phy.h"

//----------------------------------------------------------------------------//
int enet_init(void *pri, uint8_t *macAddr)
{
    struct enet_if_port *port = pri;
    enet_config_t config;
    volatile uint32_t count = port->phy_autonego_timeout_count;
    bool link               = false;
    bool autonego           = false;
    status_t status;
    phy_speed_t speed;
    phy_duplex_t duplex;
    ENET_GetDefaultConfig(&config);
    config.miiMode = port->mii_mode;
    do
    {
        status = PHY_Init(&port->phyHandle, &port->phy_config);
        if (status == kStatus_Success)
        {
            EC_PRINT("Wait for PHY link up...\r\n");
            do
            {
                PHY_GetAutoNegotiationStatus(&port->phyHandle, &autonego);
                PHY_GetLinkStatus(&port->phyHandle, &link);
                if (autonego && link)
                {
                    break;
                }
                if (port->phy_autonego_timeout_count)
                    count--;
            } while (count);
            if (!autonego)
            {
                EC_PRINT(
                    "PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
                return -1;
            }
        }
    } while (!(link && autonego));

    if (port->phy_stability_delay_us)
        SDK_DelayAtLeastUs(port->phy_stability_delay_us, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /*Get the actual PHY link speed*/
    PHY_GetLinkSpeedDuplex(&port->phyHandle, &speed, &duplex);
    config.miiSpeed  = (enet_mii_speed_t)speed;
    config.miiDuplex = (enet_mii_duplex_t)duplex;
    ENET_Init(port->base, &port->g_handle, &config, port->bufferConfig, &macAddr[0], port->srcClock_Hz);
    ENET_ActiveRead(port->base);
    return 0;
}

int enet_send(void *pri, const void *payload, uint32_t len)
{
    struct enet_if_port *port = pri;
    if (ENET_SendFrame(port->base, &port->g_handle, payload, len, 0, false, NULL) == kStatus_Success)
        return len;
    else
        return 0;
}

int enet_recv(void *pri, void *buf, uint32_t len)
{
    status_t status;
    struct enet_if_port *port = pri;
    uint32_t length;
    status = ENET_GetRxFrameSize(&port->g_handle, &length, 0);
    if (length != 0)
    {
        if (length > len)
            length = len;
        if ((status = ENET_ReadFrame(port->base, &port->g_handle, buf, length, 0, NULL)) == kStatus_Success)
        {
            return length;
        }
    }
    else if (status == kStatus_ENET_RxFrameError)
    {
        ENET_ReadFrame(port->base, &port->g_handle, NULL, 0, 0, NULL);
    }
    return 0;
}

int enet_link_status(void *pri)
{
    bool link;
    struct enet_if_port *port = pri;
    if (kStatus_Success == PHY_GetLinkStatus(&port->phyHandle, &link))
    {
        if (link)
            return 1;
        else
            return 0;
    }
    return -1;
}

int enet_close(void *pri)
{
    return 0;
}
