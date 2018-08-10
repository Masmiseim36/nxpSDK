/**
 * @file tst_utils_kinetis.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2017-2018
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 *
 * @par Description
 * Initialize LWIP / Ethernet / DHCP Connection on board
 * Set and Get Flag in GP Storage
 * vApplicationGetIdleTaskMemory and vApplicationGetTimerTaskMemory
 * json utility function
 */

/*******************************************************************************
 * includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

#ifdef USE_RTOS

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "ksdk_mbedtls.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "HLSEAPI.h"
#include "tst_utils_kinetis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0xB3, 0x10, 0x90 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

/* Facilitate a simple hash for unique MAC Address based on an input 18 byte UID */
#define MAC_HASH(N)  \
    fsl_enet_config0.macAddress[N] = buffer[(N+2)+(5*0)] ^ buffer[(N+2)+(5*1)] ^ buffer[(N+2)+(5*2)]

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static struct netif fsl_netif0;

/*******************************************************************************
 * Global variables
 ******************************************************************************/
HLSE_OBJECT_HANDLE Gpstorage_handle;

/*******************************************************************************
 * Global Function Definitions
 ******************************************************************************/

/*Init the board network */
void BOARD_InitNetwork(const unsigned char buffer[18])
{
#if FSL_FEATURE_SOC_ENET_COUNT > 0
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = EXAMPLE_PHY_ADDRESS, .clockName = EXAMPLE_CLOCK_NAME, .macAddress = configMAC_ADDR,
    };
    if (buffer != NULL)
    {
        MAC_HASH(1);
        MAC_HASH(2);
        MAC_HASH(3);
        MAC_HASH(4);
        MAC_HASH(5);
    }

    PRINTF("Connecting to network\r\n");
    tcpip_init(NULL, NULL);

    IP4_ADDR(&fsl_netif0_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&fsl_netif0_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&fsl_netif0_gw, 0U, 0U, 0U, 0U);

    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0, ethernetif0_init,
              tcpip_input);
    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);

    PRINTF("Getting IP address from DHCP ...\n");
    dhcp_start(&fsl_netif0);

    struct dhcp *dhcp;
    dhcp = (struct dhcp *)netif_get_client_data(&fsl_netif0, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    while (dhcp->state != DHCP_STATE_BOUND)
    {
        vTaskDelay(1000);
    }

    if (dhcp->state == DHCP_STATE_BOUND)
    {
        PRINTF("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *) &fsl_netif0.ip_addr.addr)[0],
            ((u8_t *) &fsl_netif0.ip_addr.addr)[1], ((u8_t *) &fsl_netif0.ip_addr.addr)[2],
            ((u8_t *) &fsl_netif0.ip_addr.addr)[3]);
    }
    PRINTF("DHCP OK\r\n");
#endif /* FSL_FEATURE_SOC_ENET_COUNT > 0 */
}

/*Set and Get the flag value from GP Storage */
int SetGetFlag_GPstorage(U32 *p_val, GpStorageMode_t mode, HLSE_OBJECT_HANDLE handle)
{
    int ret = 1;
    HLSE_RET_CODE hlseRc;
    HLSE_ATTRIBUTE attr;
    uint8_t tempBuffer[32] = {0};
    memcpy((void *)tempBuffer,(void *)p_val,sizeof(*p_val));

    attr.type = HLSE_ATTR_OBJECT_VALUE;
    attr.value = tempBuffer;
    attr.valueLen = 32;
    if (mode == SET)
    {
        hlseRc = HLSE_SetObjectAttribute(handle, &attr);
    }
    else if (mode == GET)
    {
        attr.value = tempBuffer;
        hlseRc = HLSE_GetObjectAttribute(handle, &attr);
        if (hlseRc == HLSE_SW_OK)
        {
            memcpy((void *)p_val,(void *)tempBuffer,sizeof(*p_val));
        }
    }
    else
    {
        hlseRc = 0;
    }
    if (hlseRc == HLSE_SW_OK)
    {
        ret = 0;
    }

    return ret;
}

static int GetHandle_GPstorageType(HLSE_OBJECT_INDEX index, HLSE_OBJECT_TYPE objType) {
    HLSE_RET_CODE hlseret;
    HLSE_OBJECT_HANDLE Handles[5];
    U16 HandlesNum = sizeof(Handles) / sizeof(HLSE_OBJECT_HANDLE);
    U16 HandlesNum_copy;
    U8 i = 0;
    int ret = 1;

    hlseret = HLSE_EnumerateObjects(objType, Handles, &HandlesNum);
    if (hlseret == HLSE_SW_OK)
    {
        HandlesNum_copy = HandlesNum;
        while(HandlesNum_copy)
        {
            if (HLSE_GET_OBJECT_INDEX(Handles[i]) == index)
            {
                Gpstorage_handle = Handles[i];
                ret = 0;
                break;
            }
            i++;
            HandlesNum_copy--;
        }
    }
    return ret;
}

/*Get the Handle of the GP storage */
int GetHandle_GPstorage(HLSE_OBJECT_INDEX index)
{
    /* New way. 3rd entry in table would be HLSE_DATA  */
    int ret = GetHandle_GPstorageType(index, HLSE_DATA);
    if ( ret == 1 )
    {
        /* Old way. 3rd entry in table is HLSE_CERTIFICATE */
        ret = GetHandle_GPstorageType(index, HLSE_CERTIFICATE);
    }
    if ( ret == 1 )
    {
        /* iMX UL Way. No entry. So insert 3rd entry in table. */
        HLSE_OBJECT_TYPE objType = HLSE_DATA;
        U16 templateSize = 3;
        HLSE_ATTRIBUTE attr[3];
        U8  objData[] = {0xFF,0xFF,0xFF,0xFF};
        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = &objData;
        attr[2].valueLen = 4;

        ret = HLSE_CreateObject(attr, templateSize, &Gpstorage_handle);
    }
    return ret;
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/*JSON utility function to check equality */
int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if(tok->type == JSMN_STRING) {
        if((int) strlen(s) == tok->end - tok->start) {
            if(strncmp(json + tok->start, s, (size_t) (tok->end - tok->start)) == 0) {
                return 0;
            }
        }
    }
    return -1;
}

#endif /* USE_RTOS */
