/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// wlan_qca400x declarations
int wlan_driver_start(void);
void wlan_hardware_gt202_init(void);
QCA_CONTEXT_STRUCT *wlan_get_context(void);

// wlan_qcom declarations
void printHeapFree(const char *s);
int traceQcomApi(int enable);
void apScan(void);
void apConnect(const char *ssid, const char *passphrase, uint8_t auth, uint8_t cipher);
void apDisconnect(void);
int isConnected(void);
void printIpConfig(void);
void getDhcp(void);
void pingGateway(void);
uint32_t pingHost(const char *hostname);
void resolveManyHosts(void);

void udpBind(uint16_t port);
int udpPollAndRecv(int timeout);
int udpSendEcho(void);
void httpGet(const char *hostname, int timeout);
