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
#include "mb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_START_ADDR                  0		/*LED start address*/
#define LED_NUMS                        2		    /*LED numbers*/

static uint16_t     led1_status;
static uint16_t     led2_status;

#define EXAMPLE_EP_TXFRAME_NUM 20U
#define EXAMPLE_TX_RX_INTERRUPT_HANDLE
#define INIT_THREAD_STACKSIZE 1024

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

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint16_t     inputRegStart = INPUT_REGISTER_START_ADDR;
static uint16_t     inputRegArrays[INPUT_REGISTER_NUMS];
static uint16_t     holdingRegStart = HOLDING_REGISTER_START_ADDR;
static uint16_t     holdingRegArrays[HOLDING_REGISTER_NUMS];
static uint16_t     initArrays[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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

/**
 * Modbus slave input register callback function.
 * @param pucRegBuffer input register buffer	
 * @param usAddress input register address		
 * @param usNRegs input register number			
 * @return result
 */
eMBErrorCode
eMBRegInputCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
 
    /* it already plus one in modbus function method. */
    usAddress--;
 
    if( ( usAddress >= INPUT_REGISTER_START_ADDR )
        && ( usAddress + usNRegs <= INPUT_REGISTER_START_ADDR + INPUT_REGISTER_NUMS ) )
    {
        iRegIndex = ( int )( usAddress - inputRegStart );
        while( usNRegs > 0 )
        {
            inputRegArrays[iRegIndex] = iRegIndex;
            *pucRegBuffer++ = ( unsigned char )( inputRegArrays[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( inputRegArrays[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 * Modbus slave LED status callback function.	
 * @param pucRegBuffer LED status buffer	
 * @param usAddress LED address	
 * @param usNRegs LED number		
 * @param eMode read or write					
 * @return result
 */
eMBErrorCode
eMBLedStatusCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    /* it already plus one in modbus function method. */
    usAddress--;

    if( usAddress +  usNRegs <= LED_NUMS )
    {
        if ( eMode == MB_REG_READ)
        {
            while( usNRegs > 0 )
            {
                if (usAddress == 0)
                {
                    *pucRegBuffer++ = ( uint8_t ) ( led1_status >> 8 );
                    *pucRegBuffer++ = ( uint8_t ) ( led1_status & 0xFF );
                }
                else
                {
                    *pucRegBuffer++ = ( uint8_t ) ( led2_status >> 8 );
                    *pucRegBuffer++ = ( uint8_t ) ( led2_status & 0xFF );
                }
                usAddress++;
                usNRegs--;
            }  
        } 
        else 
        {
            while( usNRegs > 0 )
            {
                if (usAddress == 0)
                {
                    led1_status =  *pucRegBuffer++ << 8;
                    led1_status |= *pucRegBuffer++;
                    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN1, led1_status);
                } 
                else
                {
                    led2_status =  *pucRegBuffer++ << 8;
                    led2_status |= *pucRegBuffer++;
                    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN2, led2_status);
                }
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}
 
/**
 * Modbus slave holding register callback function.	
 * @param pucRegBuffer holding register buffer	
 * @param usAddress holding register address	
 * @param usNRegs holding register number		
 * @param eMode read or write					
 * @return result
 */
eMBErrorCode
eMBRegHoldingCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
 
    /* it already plus one in modbus function method. */
    usAddress--;
    if( ( usAddress >= HOLDING_REGISTER_START_ADDR ) &&
        ( usAddress + usNRegs <= HOLDING_REGISTER_START_ADDR + HOLDING_REGISTER_NUMS ) )
    {
        iRegIndex = ( int )( usAddress - holdingRegStart );
        if ( eMode == MB_REG_READ)
        {
            while( usNRegs > 0 )
            {
                holdingRegArrays[iRegIndex] = initArrays[iRegIndex];
                *pucRegBuffer++ = ( uint8_t ) ( holdingRegArrays[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( uint8_t ) ( holdingRegArrays[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
        } 
        else 
        {
            while( usNRegs > 0 )
            {
                holdingRegArrays[iRegIndex] = *pucRegBuffer++ << 8;
                holdingRegArrays[iRegIndex] |= *pucRegBuffer++;
                initArrays[iRegIndex] = holdingRegArrays[iRegIndex];
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
 
eMBErrorCode
eMBRegCoilsCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNCoils, eMBRegisterMode eMode )
{
    return MB_ENOREG;
}
 
eMBErrorCode
eMBRegDiscreteCB( uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNDiscrete )
{
    return MB_ENOREG;
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

    /* Init Modbus TCP */
    LOCK_TCPIP_CORE();
    eMBTCPInit( MB_TCP_PORT_USE_DEFAULT );
    UNLOCK_TCPIP_CORE();

    /* Enable Modbus TCP */
    LOCK_TCPIP_CORE();
    eMBEnable();
    UNLOCK_TCPIP_CORE();
    
    for( ;; ) {
        /* Enable the Modbus Protocol Stack. */
        ( void )eMBPoll( );
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
    PRINTF(" Modbus TCP Server example\r\n");
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

    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t led1_config = {
        kRGPIO_DigitalOutput,
        0,
    };
    rgpio_pin_config_t led2_config = {
        kRGPIO_DigitalOutput,
        0,
    };

    /* Init output LED GPIO. */
    RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN1, &led1_config);
    RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN2, &led2_config);

    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN1, 1);
    RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN2, 1);
    
    /* Initialize lwIP from thread */
    if (sys_thread_new("stack_init", stack_init, NULL, INIT_THREAD_STACKSIZE, TASK_PRIORITY) == NULL)
    {
        LWIP_ASSERT("stack_init(): Task creation failed.", 0);
    }

    /* run RTOS */
    vTaskStartScheduler();
    return 0;
}

