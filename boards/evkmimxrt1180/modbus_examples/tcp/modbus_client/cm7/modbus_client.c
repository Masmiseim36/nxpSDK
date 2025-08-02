/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip/opt.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"

#include "lwip/tcp.h"
#include "lwip/dhcp.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "ethernetif.h"

#include "fsl_phy.h"
#include "fsl_adapter_gpio.h"

/* Must be after include of app.h */
#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif

/* ------------------------ FreeModbus includes --------------------------- */
// #include "mb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_EP_TXFRAME_NUM 20U
#define EXAMPLE_TX_RX_INTERRUPT_HANDLE
#define INIT_THREAD_STACKSIZE 1024

#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_LED_STATUS              ( 13 )
#define MB_FUNC_READ_LED_STATUS               ( 14 )

#define COIL_START_ADDR                 0           /*coil register start address*/
#define COIL_NUMS                       10          /*coil register numbers*/
 
#define DISCRETE_INPUT_START_ADDR       10000	    /*discrete register start address*/
#define DISCRETE_INPUT_NUMS             10          /*discrete register numbers*/
 
#define INPUT_REGISTER_START_ADDR       30000		/*input register start address*/
#define INPUT_REGISTER_NUMS             10		    /*input register numbers*/
 
#define HOLDING_REGISTER_START_ADDR     40000		/*holding register start address*/
#define HOLDING_REGISTER_NUMS           10		    /*holding register numbers*/

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#ifndef HTTPD_STACKSIZE
#define HTTPD_STACKSIZE DEFAULT_THREAD_STACKSIZE
#endif

#ifndef HTTPD_PRIORITY
#define HTTPD_PRIORITY DEFAULT_THREAD_PRIO
#endif

/* ------------------------- MODBUS -----------------------------------------*/
#define PROG                    "FreeModbus"
#define MODBUS_TCP_CLIENT_PORT 502
#define MODBUS_TCP_SERVER_PORT 502
#define MB_TCP_BUF_SIZE     ( 256 + 7 ) /* Must hold a complete Modbus TCP frame. */

/* ----------------------- MBAP Header --------------------------------------*/
#define MB_TCP_HEADER_LEN               7
#define MB_TCP_TRANSACTION_HIGH         0
#define MB_TCP_TRANSACTION_LOW          1
#define MB_TCP_PROTOCOL_HIGH            2
#define MB_TCP_PROTOCOL_LOW             3
#define MB_TCP_LENGTH_HIGH              4
#define MB_TCP_LENGTH_LOW               5
#define MB_TCP_UID                      6

/* ----------------------- Modbus PDU --------------------------------------*/
#define MB_TCP_FUNCTION_CODE            7
#define MB_TCP_START_ADDR_HIGH          8
#define MB_TCP_START_ADDR_LOW           9
#define MB_TCP_LED_DATA_HIGH            10
#define MB_TCP_LED_DATA_LOW             11

/* ----------------------- Defines  -----------------------------------------*/
/*! @brief Selection of GPIO perihperal and its pin for the reception of PHY interrupts. */
#if ETH_LINK_POLLING_INTERVAL_MS == 0
#ifndef EXAMPLE_PHY_INT_PORT
#if (!defined(BOARD_NETWORK_USE_100M_ENET_PORT) || !BOARD_NETWORK_USE_100M_ENET_PORT) && \
    defined(BOARD_INITENET1GPINS_PHY_INTR_PERIPHERAL)
#define EXAMPLE_PHY_INT_PORT BOARD_INITENET1GPINS_PHY_INTR_PERIPHERAL
#elif defined(BOARD_INITENETPINS_PHY_INTR_PERIPHERAL)
#define EXAMPLE_PHY_INT_PORT BOARD_INITENETPINS_PHY_INTR_PERIPHERAL
#elif defined(BOARD_INITPINS_PHY_INTR_PERIPHERAL)
#define EXAMPLE_PHY_INT_PORT BOARD_INITPINS_PHY_INTR_PERIPHERAL
#else
#error "Interrupt-based link-state detection was enabled on an unsupported board."
#endif
#endif // #ifndef EXAMPLE_PHY_INT_PORT

#ifndef EXAMPLE_PHY_INT_PIN
#if (!defined(BOARD_NETWORK_USE_100M_ENET_PORT) || !BOARD_NETWORK_USE_100M_ENET_PORT) && \
    defined(BOARD_INITENET1GPINS_PHY_INTR_CHANNEL)
#define EXAMPLE_PHY_INT_PIN BOARD_INITENET1GPINS_PHY_INTR_CHANNEL
#elif defined(BOARD_INITENETPINS_PHY_INTR_CHANNEL)
#define EXAMPLE_PHY_INT_PIN BOARD_INITENETPINS_PHY_INTR_CHANNEL
#elif defined(BOARD_INITPINS_PHY_INTR_CHANNEL)
#define EXAMPLE_PHY_INT_PIN BOARD_INITPINS_PHY_INTR_CHANNEL
#else
#error "Interrupt-based link-state detection was enabled on an unsupported board."
#endif
#endif // #ifndef EXAMPLE_PHY_INT_PIN
#endif // #if ETH_LINK_POLLING_INTERVAL_MS == 0

#define PROG                    "FreeModbus"

#define TASK_PRIORITY DEFAULT_THREAD_PRIO

static netif_ext_callback_t linkStatusCallbackInfo;

static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

static StaticTask_t IdleTaskTCB;

static StackType_t TimerTaskStack[configMINIMAL_STACK_SIZE];

static StaticTask_t TimerTaskTCB;

static uint8_t    sendTCPBuf[MB_TCP_BUF_SIZE];

static uint8_t    recvTCPBuf[MB_TCP_BUF_SIZE];

struct tcp_pcb *tcpPcb;

uint16_t mbTCPWriteLen = 0;

char input_ip[16];

int ip_addr[4];
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* PHY operation. */

static phy_handle_t phyHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = &IdleTaskStack[0];
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer   = &TimerTaskTCB;
    *ppxTimerTaskStackBuffer = &TimerTaskStack[0];
    *pulTimerTaskStackSize   = configMINIMAL_STACK_SIZE;
}

/*!
 * @brief Link status callback - prints link status events.
 */
static void linkStatusCallback(struct netif *netif_, netif_nsc_reason_t reason, const netif_ext_callback_args_t *args)
{
    if (reason != LWIP_NSC_LINK_CHANGED)
        return;

    PRINTF("[LINK STATE] netif=%d, state=%s", netif_->num, args->link_changed.state ? "up" : "down");

    if (args->link_changed.state)
    {
        char *speedStr;
        switch (ethernetif_get_link_speed(netif_))
        {
            case kPHY_Speed10M:
                speedStr = "10M";
                break;
            case kPHY_Speed100M:
                speedStr = "100M";
                break;
            case kPHY_Speed1000M:
                speedStr = "1000M";
                break;
            default:
                speedStr = "N/A";
                break;
        }

        char *duplexStr;
        switch (ethernetif_get_link_duplex(netif_))
        {
            case kPHY_HalfDuplex:
                duplexStr = "half";
                break;
            case kPHY_FullDuplex:
                duplexStr = "full";
                break;
            default:
                duplexStr = "N/A";
                break;
        }

        PRINTF(", speed=%s_%s", speedStr, duplexStr);
    }

    PRINTF("\r\n");
}

int check_ip_addr(char *ip)
{
	if (ip == NULL || ip[0] == '0' || ip[0] == '\0') {
		return -1;
	}

	for (int i = 0, count = 0; i < strlen(ip); i++) {
		if ((ip[i] != '.') && (ip[i] < '0' || ip[i] > '9')) {
			return -1;
		}
		if (ip[i] == '.') {
			count++;
			if (count > 3) {
				return -1;
			}
		}
	}

	char ip_check[4][4];
	memset(ip_check, 0, sizeof(char[4]) * 4);

	sscanf(ip, "%[^.].%[^.].%[^.].%[^ ]", ip_check[0], ip_check[1], ip_check[2], ip_check[3]);
    ip_addr[0] = atoi(ip_check[0]);
    ip_addr[1] = atoi(ip_check[1]);
    ip_addr[2] = atoi(ip_check[2]);
    ip_addr[3] = atoi(ip_check[3]);

	for (int i = 0; i < 4; i++) {
		if ((strlen(ip_check[i])) == 0 || (ip_check[i][0] == '0' && ip_check[i][1] != '\0') || ip_addr[i] < 0 || ip_addr[i] > 255) {
			return -1;
		}
	}

	return 0;
}

/**
 * Modbus Client write holding register function.	
 * @param addr holding register addr	
 * @param data write holding register data					
 * @return result
 */
err_t eMBMasterRegHoldingCB(uint16_t addr, uint16_t data)
{
    sendTCPBuf[MB_TCP_TRANSACTION_HIGH] = 0x0;
    sendTCPBuf[MB_TCP_TRANSACTION_LOW] = 0x1;
    sendTCPBuf[MB_TCP_PROTOCOL_HIGH] = 0x0;
    sendTCPBuf[MB_TCP_PROTOCOL_LOW] = 0x0;
    sendTCPBuf[MB_TCP_LENGTH_HIGH] = 6 >> 8;
    sendTCPBuf[MB_TCP_LENGTH_LOW] = 6 & 0xFF;
    sendTCPBuf[MB_TCP_UID] = 0x1;
    sendTCPBuf[MB_TCP_FUNCTION_CODE] = MB_FUNC_WRITE_REGISTER;
    sendTCPBuf[MB_TCP_START_ADDR_HIGH] = ( uint8_t )( addr >> 8 );
    sendTCPBuf[MB_TCP_START_ADDR_LOW] = ( uint8_t )( addr & 0xFF );
    sendTCPBuf[MB_TCP_LED_DATA_HIGH] = ( uint8_t )( data >> 8 );
    sendTCPBuf[MB_TCP_LED_DATA_LOW] = ( uint8_t )( data & 0xFF );
    mbTCPWriteLen = 12;

    return ERR_OK;
}

/**
 * Modbus Client write LED status function.	
 * @param addr LED addr	
 * @param data write LED status data					
 * @return result
 */
err_t eMBMasterWriteLEDCB(uint16_t addr, uint16_t data)
{
    sendTCPBuf[MB_TCP_TRANSACTION_HIGH] = 0x0;
    sendTCPBuf[MB_TCP_TRANSACTION_LOW] = 0x1;
    sendTCPBuf[MB_TCP_PROTOCOL_HIGH] = 0x0;
    sendTCPBuf[MB_TCP_PROTOCOL_LOW] = 0x0;
    sendTCPBuf[MB_TCP_LENGTH_HIGH] = 6 >> 8;
    sendTCPBuf[MB_TCP_LENGTH_LOW] = 6 & 0xFF;
    sendTCPBuf[MB_TCP_UID] = 0x1;
    sendTCPBuf[MB_TCP_FUNCTION_CODE] = MB_FUNC_WRITE_LED_STATUS;
    sendTCPBuf[MB_TCP_START_ADDR_HIGH] = ( uint8_t )( addr >> 8 );
    sendTCPBuf[MB_TCP_START_ADDR_LOW] = ( uint8_t )( addr & 0xFF );
    sendTCPBuf[MB_TCP_LED_DATA_HIGH] = ( uint8_t )( data >> 8 );
    sendTCPBuf[MB_TCP_LED_DATA_LOW] = ( uint8_t )( data & 0xFF );
    mbTCPWriteLen = 12;

    return ERR_OK;
}

static err_t tcp_recv_callback( void *pvArg, struct tcp_pcb *pxPCB, struct pbuf *p, err_t xErr)
{
    uint16_t frameLen;
    uint8_t i;
    tcp_recved(pxPCB, p->tot_len);
    memcpy( &recvTCPBuf[0], p->payload, p->len );

    if (recvTCPBuf[MB_TCP_PROTOCOL_HIGH] != 0x0 || recvTCPBuf[MB_TCP_PROTOCOL_LOW] != 0x0)
    {
        PRINTF("Not is Modbus TCP Frame!\r\n");
        return ERR_VAL;
    }
    
    frameLen = (recvTCPBuf[MB_TCP_LENGTH_HIGH] << 8) + recvTCPBuf[MB_TCP_LENGTH_LOW];

    /*MBAP*/
    for (i = 0; i < MB_TCP_HEADER_LEN; i++)
    {
        PRINTF("%d ", recvTCPBuf[i]);
    }

    for (i = 0; i < (frameLen - 1); i++)
    {
        PRINTF("%d ", recvTCPBuf[MB_TCP_HEADER_LEN + i]);
    }
    PRINTF("\r\n");

    pbuf_free(p);

    return ERR_OK;
}

static err_t tcp_conn(void *arg, struct tcp_pcb *pcb, err_t err)
{

    tcp_recv(pcb, tcp_recv_callback);

    return ERR_OK;
}

err_t tcp_client_init(void)
{
    ip_addr_t serverIp;
    err_t err;

    IP4_ADDR(&serverIp, ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);

    tcpPcb = tcp_new();

    if (tcpPcb)
    {
        err = tcp_bind(tcpPcb, IP_ADDR_ANY, MODBUS_TCP_CLIENT_PORT);

        if (err != ERR_OK)
        {
            PRINTF("tcp_bind failure\r\n");
            memp_free(MEMP_TCP_PCB, tcpPcb);
        }

        tcp_connect(tcpPcb, &serverIp, MODBUS_TCP_SERVER_PORT, tcp_conn);
        
    }
    return err;
}

static void modbus_task(void *arg)
{
    struct netif *netif = (struct netif *)arg;
    struct dhcp *dhcp;
    int Time_dhcp=0;

    /* Wait for address from DHCP */
    PRINTF("FOR TASK ETH Get IP address from DHCP...\r\n");

    do {
        if (netif_is_up(netif))
    {
        dhcp = netif_dhcp_data(netif);
    } else {
        dhcp = NULL;
    }
        sys_msleep(20U);
        Time_dhcp++;
    } while ((dhcp == NULL) || (dhcp->state != DHCP_STATE_BOUND)|| Time_dhcp <= 30);

    PRINTF("\r\nIPv4 Address : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
    PRINTF("IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
    PRINTF("IPv4 Gateway : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));

    LOCK_TCPIP_CORE();
    tcp_client_init();
    UNLOCK_TCPIP_CORE();

    for( ;; ) {
       
        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        PRINTF("Enable LED1\r\n");
        eMBMasterWriteLEDCB(0, 1);
        LOCK_TCPIP_CORE();
        tcp_write( tcpPcb, sendTCPBuf, mbTCPWriteLen, TCP_WRITE_FLAG_COPY );
        tcp_output(tcpPcb);
        UNLOCK_TCPIP_CORE();

        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        PRINTF("Enable LED2\r\n");
        eMBMasterWriteLEDCB(1, 1);
        LOCK_TCPIP_CORE();
        tcp_write( tcpPcb, sendTCPBuf, mbTCPWriteLen, TCP_WRITE_FLAG_COPY );
        tcp_output(tcpPcb);
        UNLOCK_TCPIP_CORE();

        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        PRINTF("Disable LED1\r\n");
        eMBMasterWriteLEDCB(0, 0);
        LOCK_TCPIP_CORE();
        tcp_write( tcpPcb, sendTCPBuf, mbTCPWriteLen, TCP_WRITE_FLAG_COPY );
        tcp_output(tcpPcb);
        UNLOCK_TCPIP_CORE();

        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        PRINTF("Disable LED2\r\n");
        eMBMasterWriteLEDCB(1, 0);
        LOCK_TCPIP_CORE();
        tcp_write( tcpPcb, sendTCPBuf, mbTCPWriteLen, TCP_WRITE_FLAG_COPY );
        tcp_output(tcpPcb);
        UNLOCK_TCPIP_CORE();

    }
}

/*!
 * @brief Initializes lwIP stack.
 */
static void stack_init(void *arg)
{
    static struct netif netif;
    ethernetif_config_t enet_config = {
        .phyHandle   = &phyHandle,
        .phyAddr     = EXAMPLE_PHY_ADDRESS,
        .phyOps      = EXAMPLE_PHY_OPS,
        .phyResource = EXAMPLE_PHY_RESOURCE,
        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
        .macAddress = configMAC_ADDR,
#endif
#if ETH_LINK_POLLING_INTERVAL_MS == 0
        .phyIntGpio    = EXAMPLE_PHY_INT_PORT,
        .phyIntGpioPin = EXAMPLE_PHY_INT_PIN
#endif
    };

    LWIP_UNUSED_ARG(arg);

    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

    HAL_GpioPreInit();

    tcpip_init(NULL, NULL);

    LOCK_TCPIP_CORE();
    netif_add_ext_callback(&linkStatusCallbackInfo, linkStatusCallback);
    UNLOCK_TCPIP_CORE();

    netifapi_netif_add(&netif, NULL, NULL, NULL, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    netifapi_dhcp_start(&netif);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" Modbus TCP example\r\n");
    PRINTF("************************************************\r\n");

    if (sys_thread_new("modbus_task", modbus_task, &netif, INIT_THREAD_STACKSIZE, TASK_PRIORITY) == NULL)
    {
        LWIP_ASSERT("modbus_task(): Task creation failed.", 0);
    }

    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("Please input Modbus Server ip address(for example 192.168.1.2):\r\n");
    PRINTF("Note: don't use backspace key or space key!\r\n");
    while (1)
    {
        SCANF("%s", input_ip);
        PRINTF("Modbus Server ip: %s\r\n", input_ip);
        if (check_ip_addr(input_ip) == 0)
        {
            break;
        }
        PRINTF("Please input valid ip address!\r\n");
    }
    
    /* Initialize lwIP from thread */
    if (sys_thread_new("stack_init", stack_init, NULL, INIT_THREAD_STACKSIZE, TASK_PRIORITY) == NULL)
    {
        LWIP_ASSERT("stack_init(): Task creation failed.", 0);
    }

    /* run RTOS */
    vTaskStartScheduler();
    return 0;
}

