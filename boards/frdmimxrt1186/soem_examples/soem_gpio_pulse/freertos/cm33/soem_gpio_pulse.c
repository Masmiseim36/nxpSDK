/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "nicdrv.h"
#include "ethercattype.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "soem_port.h"
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_gpt.h"
#include "app.h"

#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
#include "netc_ep/soem_netc_ep.h"
#include "netc_ep/netc_ep.h"
#endif
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
#include "netc_swt/soem_netc_swt.h"
#include "netc_swt/netc_swt.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NUM_1M      (1000000UL)

#define BOARD_IMX_RT1180 (1)

#define EC_TIMEOUTMON  500

#define SOEM_PERIOD        1000 /* 500us */
#define RT_TASK_STACK_SIZE 1024

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (500000U)
#endif

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
/* ENETC pseudo port for management */
#ifndef EXAMPLE_SWT_SI
#define EXAMPLE_SWT_SI kNETC_ENETC1PSI0
#endif
/* Switch pseudo port */
#ifndef EXAMPLE_SWT_PSEUDO_PORT
#define EXAMPLE_SWT_PSEUDO_PORT 0x4U
#endif
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Rx buffer memeory type. */
typedef uint8_t rx_buffer_t[EP_RXBUFF_SIZE_ALIGN];

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
extern struct netc_ep_if_port if_port;
#endif

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
extern struct netc_swt_if_port if_port;
#endif

static uint32_t timer_irq_period = 0; /* unit: microsecond*/

volatile struct timeval system_time_base = {.tv_sec = 0, .tv_usec = 0};

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
/* SWT resource. */
static swt_config_t g_swt_config;
static swt_transfer_config_t swtTxRxConfig;
#endif

/* Buffer descriptor resource. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EP_RING_NUM][EP_RXBD_NUM],
                              EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EP_RING_NUM][EP_RXBD_NUM],
                              EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EP_TXBUFF_SIZE], EP_BUFF_SIZE_ALIGN);
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EP_TXBD_NUM], EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EP_TXBD_NUM], EP_BD_ALIGN);
#endif
static uint64_t rxBuffAddrArray[EP_RING_NUM][EP_RXBD_NUM];
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
static netc_tx_frame_info_t g_mgmtTxDirty[EP_TXBD_NUM];
#endif
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EP_RING_NUM][EP_TXBD_NUM],
                              EP_BD_ALIGN);
static netc_tx_frame_info_t g_txDirty[EP_RING_NUM][EP_TXBD_NUM];
#endif

static netc_rx_bdr_config_t rxBdrConfig = {0};
static netc_tx_bdr_config_t txBdrConfig = {0};
static netc_bdr_config_t bdrConfig;
static netc_msix_entry_t msixEntry[2];
static ep_config_t ep_config;

static char IOmap[100];

static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
static StaticTask_t IdleTaskTCB;

static StackType_t TimerTaskStacj[configMINIMAL_STACK_SIZE];
static StaticTask_t TimerTaskTCB;

static StaticTask_t xTaskBuffer;
static TaskHandle_t rt_task = NULL;
static StackType_t rt_task_stack[RT_TASK_STACK_SIZE];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   configSTACK_DEPTH_TYPE *pulIdleTaskStackSize)
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
    *ppxTimerTaskStackBuffer = &TimerTaskStacj[0];
    *pulTimerTaskStackSize   = configMINIMAL_STACK_SIZE;
}

void irq_wake_task(void);

void OSAL_TIMER_IRQHandler(void)
{
    /* Clear interrupt flag. */
    GPT_ClearStatusFlags(OSAL_TIMER, kGPT_OutputCompare1Flag);

    system_time_base.tv_usec += timer_irq_period;

    if (system_time_base.tv_usec >= NUM_1M)
    {
        system_time_base.tv_sec += system_time_base.tv_usec / NUM_1M;
        system_time_base.tv_usec = system_time_base.tv_usec % NUM_1M;
    }

    irq_wake_task();
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
 	 exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}

static void osal_timer_init(uint32_t usec, uint32_t priority)
{
    uint32_t gptFreq;
    gpt_config_t gptConfig;

    assert(usec != 0);
    assert(priority < (1UL << __NVIC_PRIO_BITS));

    timer_irq_period = usec;
    GPT_GetDefaultConfig(&gptConfig);
    GPT_Init(OSAL_TIMER, &gptConfig);
    gptFreq = OSAL_TIMER_CLK_FREQ;
    /* Divide GPT clock source frequency to 1MHz */
    GPT_SetClockDivider(OSAL_TIMER, gptFreq / NUM_1M);
    /* Set both GPT modules to 1 second duration */
    GPT_SetOutputCompareValue(OSAL_TIMER, kGPT_OutputCompare_Channel1, timer_irq_period);
    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(OSAL_TIMER, kGPT_OutputCompare1InterruptEnable);
    /* Enable at the Interrupt */
    NVIC_SetPriority(OSAL_TIMER_IRQ_ID, priority);
    EnableIRQ(OSAL_TIMER_IRQ_ID);
    GPT_StartTimer(OSAL_TIMER);
}

void msgintrCallback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
    /* Transmit interrupt */
    if ((pendingIntr & (1U << TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&if_port.g_ep_handle, 1, 0);
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
        EP_ReclaimTxDescriptor(&if_port.g_ep_handle, 0);
#endif
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)    
        if_port.txOver = true;
#endif
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(&if_port.g_ep_handle, 1);
    }
}

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
static status_t APP_SwtReclaimCallback(swt_handle_t *handle, netc_tx_frame_info_t *frameInfo, void *userData)
{
    return kStatus_Success;
}
#endif

void osal_gettime(struct timeval *current_time)
{
    uint32_t usec_base;
    uint32_t cur_usec;
    uint32_t usec_again;

    usec_base  = system_time_base.tv_usec;
    cur_usec   = GPT_GetCurrentTimerCount(OSAL_TIMER);
    usec_again = system_time_base.tv_usec;

    if (usec_again != usec_base)
    {
        usec_base = system_time_base.tv_usec;
        cur_usec  = GPT_GetCurrentTimerCount(OSAL_TIMER);
    }

    current_time->tv_sec  = system_time_base.tv_sec;
    current_time->tv_usec = usec_base + cur_usec;

    return;
}

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
static status_t APP_SWT_AddTableEntry()
{
    status_t result = kStatus_Success;
    uint32_t entryID;

#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
    netc_tb_ipf_config_t ipfEntryCfg;

    memset(&ipfEntryCfg, 0U, sizeof(netc_tb_ipf_config_t));
    ipfEntryCfg.keye.srcPortMask = EXAMPLE_SWT_USED_PORT_BITMAP;
    ipfEntryCfg.cfge.hr          = kNETC_SoftwareDefHR0;
    ipfEntryCfg.cfge.fltfa       = kNETC_IPFRedirectToMgmtPort;

    /* Frame from used port redirect to management port. */
    for (uint32_t i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        /* Only check the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            continue;
        }

        ipfEntryCfg.keye.srcPort = i;
        result                   = SWT_RxIPFAddTableEntry(&if_port.g_swt_handle, &ipfEntryCfg, &entryID);
        if ((kStatus_Success != result) && (entryID != 0xFFFFFFFF))
        {
            PRINTF("\r\n%s: %d, Failed to add IPF table!, result = %u\r\n, entryID = %u", __func__, __LINE__, result,
                   entryID);
            return kStatus_Fail;
        }
    }
#else
    /* Set FDB table, input frame only forwards to pseudo MAC port. */
    netc_tb_fdb_config_t fdbEntryCfg = {
        .keye.fid = FRAME_FID, .cfge.portBitmap = (1U << EXAMPLE_SWT_PSEUDO_PORT), .cfge.dynamic = 1};
    memset(&fdbEntryCfg.keye.macAddr[0], 0xFF, 6U);
    result = SWT_BridgeAddFDBTableEntry(&if_port.g_swt_handle, &fdbEntryCfg, &entryID);
    if ((kStatus_Success != result) || (0xFFFFFFFFU == entryID))
    {
        PRINTF("\r\n%s: %d, Failed to add FDB table!, result = %d, entryID = %d\r\n", __func__, __LINE__, result,
               entryID);
        return kStatus_Fail;
    }
#endif

    return result;
}
#endif

/* OSHW: register enet port to SOEM stack */
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
static int if_port_init(void)
{
	struct soem_if_port soem_port;

    bdrConfig.rxBdrConfig = &rxBdrConfig;
	bdrConfig.txBdrConfig = &txBdrConfig;
    uint32_t msgAddr;

    PRINTF("\r\nNETC EP frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = RX_INTR_MSG_DATA;

    /* BD ring configuration. */
    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    bdrConfig.txBdrConfig[0].bdArray      = &g_txBuffDescrip[0][0];
    bdrConfig.txBdrConfig[0].len          = EP_TXBD_NUM;
    bdrConfig.txBdrConfig[0].dirtyArray   = &g_txDirty[0][0];
    bdrConfig.txBdrConfig[0].msixEntryIdx = TX_MSIX_ENTRY_IDX;
    bdrConfig.txBdrConfig[0].enIntr       = true;

    /* Endpoint configuration. */
    (void)EP_GetDefaultConfig(&ep_config);
    ep_config.si                    = kNETC_ENETC0PSI0;
    ep_config.siConfig.txRingUse    = 1;
    ep_config.siConfig.rxRingUse    = 1;
    //ep_config.reclaimCallback       = ReclaimCallback;
    ep_config.msixEntry             = &msixEntry[0];
    ep_config.entryNum              = 2;
    ep_config.port.ethMac.miiMode   = kNETC_RmiiMode;
    ep_config.port.ethMac.miiSpeed  = kNETC_MiiSpeed100M;
    ep_config.port.ethMac.miiDuplex = kNETC_MiiFullDuplex;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    ep_config.rxCacheMaintain = true;
    ep_config.txCacheMaintain = true;
#endif
    
	for (uint8_t ring = 0U; ring < EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }
	if_port.ep_config = &ep_config;
	if_port.bdrConfig = &bdrConfig;
	if_port.g_txFrame = g_txFrame;
	strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
	strncpy(soem_port.dev_name, "net_ep", SOEM_DEV_NAME_MAXLEN);
	soem_port.port_init = netc_ep_init;
	soem_port.port_send = netc_ep_send;
	soem_port.port_recv = netc_ep_recv;
	soem_port.port_close = netc_ep_close;
	soem_port.port_link_status= netc_ep_link_status;
	soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
}
#endif

#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
static int if_port_swt_init(void)
{
	struct soem_if_port soem_port;
    bdrConfig.rxBdrConfig = &rxBdrConfig;
	bdrConfig.txBdrConfig = &txBdrConfig;
    uint32_t msgAddr;

    PRINTF("\r\nNETC swtich frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = RX_INTR_MSG_DATA;

    /* BD ring configuration. */
    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    /* Endpoint configuration. */
    (void)EP_GetDefaultConfig(&ep_config);
    ep_config.si                    = EXAMPLE_SWT_SI;
    ep_config.siConfig.txRingUse    = 1;
    ep_config.siConfig.rxRingUse    = 1;
    //ep_config.reclaimCallback       = ReclaimCallback;
    ep_config.msixEntry             = &msixEntry[0];
    ep_config.entryNum              = 2;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    ep_config.rxCacheMaintain = true;
    ep_config.txCacheMaintain = true;
#endif

    SWT_GetDefaultConfig(&g_swt_config);

    for (int i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        g_swt_config.ports[i].ethMac.miiMode   = KNETC_HW_MII_MODE;
        g_swt_config.ports[i].ethMac.miiSpeed  = kNETC_MiiSpeed100M;
        g_swt_config.ports[i].ethMac.miiDuplex = kNETC_MiiFullDuplex;
    }

    /* Wait a moment for PHY status to be stable. */
    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    g_swt_config.bridgeCfg.dVFCfg.portMembership = (1U << EXAMPLE_SWT_PSEUDO_PORT) | EXAMPLE_SWT_USED_PORT_BITMAP;
#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
    g_swt_config.ports[0].commonCfg.ipfCfg.enIPFTable = true;
    g_swt_config.ports[2].commonCfg.ipfCfg.enIPFTable = true;
#else
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID      = FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo           = kNETC_FDBLookUpWithDiscard;
    g_swt_config.bridgeCfg.dVFCfg.mlo           = kNETC_DisableMACLearn;
#endif

    g_swt_config.cmdRingUse            = 1U;
    g_swt_config.cmdBdrCfg[0].bdBase   = &g_cmdBuffDescrip[0];
    g_swt_config.cmdBdrCfg[0].bdLength = 8U;

    /* Configure switch transfer resource. */
    swtTxRxConfig.enUseMgmtRxBdRing            = false;
    swtTxRxConfig.enUseMgmtTxBdRing            = true;
    swtTxRxConfig.mgmtTxBdrConfig.bdArray      = &g_mgmtTxBuffDescrip[0];
    swtTxRxConfig.mgmtTxBdrConfig.len          = EP_TXBD_NUM;
    swtTxRxConfig.mgmtTxBdrConfig.dirtyArray   = &g_mgmtTxDirty[0];
    swtTxRxConfig.mgmtTxBdrConfig.msixEntryIdx = TX_MSIX_ENTRY_IDX;
    swtTxRxConfig.mgmtTxBdrConfig.enIntr       = true;
    swtTxRxConfig.reclaimCallback              = APP_SwtReclaimCallback;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    swtTxRxConfig.rxCacheMaintain = true;
    swtTxRxConfig.txCacheMaintain = true;
#endif

	for (uint8_t ring = 0U; ring < EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }
	if_port.ep_config = &ep_config;
    if_port.swt_config = &g_swt_config;
	if_port.swtTxRxConfig = &swtTxRxConfig;
    if_port.bdrConfig = &bdrConfig;
	if_port.g_txFrame = g_txFrame;
	strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
	strncpy(soem_port.dev_name, "netc_swt", SOEM_DEV_NAME_MAXLEN);
	soem_port.port_init = netc_swt_init;
	soem_port.port_send = netc_swt_send;
	soem_port.port_recv = netc_swt_recv;
	soem_port.port_close = netc_swt_close;
	soem_port.port_link_status= netc_swt_link_status;
	soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
}
#endif

void irq_wake_task(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    if (rt_task)
    {
        xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(rt_task, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void control_task(void *ifname)
{
    int oloop, iloop;
    int expectedWKC;
    volatile int wkc;
    int old_switch0 = 0;
    int old_switch1 = 0;
    struct timeval target_time;
    const TickType_t xBlockTime = pdMS_TO_TICKS(500);
    char chk                    = 40;

    /* initialise SOEM, and if_port */
    if (ec_init(ifname))
    {
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
        if (kStatus_Success != APP_SWT_AddTableEntry())
        {
            PRINTF("APP_SWT_AddTableEntry failed.\r\n");
            return;
        }
#endif
        PRINTF("ec_init on %s succeeded.\r\n", ifname);
        if (ec_config_init(FALSE) > 0)
        {
            PRINTF("%d slaves found and configured.\r\n", ec_slavecount);
            ec_config_map(&IOmap);
            ec_configdc();
            PRINTF("Slaves mapped, state to SAFE_OP.\r\n");
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);
            oloop = ec_slave[2].Obytes;
            iloop = ec_slave[3].Ibytes;
            PRINTF("oloop = %d, iloop = %d\r\n", oloop, iloop);
            PRINTF("segments : %d : %d %d %d %d\r\n", ec_group[0].nsegments, ec_group[0].IOsegment[0],
                   ec_group[0].IOsegment[1], ec_group[0].IOsegment[2], ec_group[0].IOsegment[3]);

            PRINTF("Request operational state for all slaves\r\n");
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            PRINTF("Calculated workcounter %d\r\n", expectedWKC);
            ec_slave[0].state = EC_STATE_OPERATIONAL;
            /* send one valid process data to make outputs in slaves happy*/
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            /* request OP state for all slaves */
            ec_writestate(0);
            /* wait for all slaves to reach OP state */
            do
            {
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            } while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
            PRINTF("Operational state reached for all slaves.\r\n");
            /* cyclic loop */
            ulTaskNotifyTake(pdTRUE, xBlockTime);
            osal_gettime(&target_time);
            for (;;)
            {
                ec_send_processdata();
                wkc = ec_receive_processdata(EC_TIMEOUTRET);
                if (wkc >= expectedWKC)
                {
                    if ((*(ec_slave[3].inputs) & 0x01) && old_switch0 == 0)
                    {
                        if (*(ec_slave[2].outputs) & 0x04)
                            *(ec_slave[2].outputs) &= ~0x04;
                        else
                            *(ec_slave[2].outputs) |= 0x04;
                    }

                    if ((*(ec_slave[3].inputs) & 0x02) && old_switch1 == 0)
                    {
                        if (*(ec_slave[2].outputs) & 0x04)
                            *(ec_slave[2].outputs) &= ~0x04;
                        else
                            *(ec_slave[2].outputs) |= 0x04;
                    }

                    old_switch0 = *(ec_slave[3].inputs) & 0x01;
                    old_switch1 = *(ec_slave[3].inputs) & 0x02;
                    if (*(ec_slave[2].outputs) & 0x02)
                        *(ec_slave[2].outputs) &= ~0x02;
                    else
                        *(ec_slave[2].outputs) |= 0x02;
                }
                ulTaskNotifyTake(pdFALSE, xBlockTime);
            }
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("Start the soem_gpio_pulse FreeRTOS example...\r\n");

    osal_timer_init(SOEM_PERIOD, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
    if_port_init();
#endif
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
    if_port_swt_init();
#endif

    rt_task = xTaskCreateStatic(/* The function that implements the task. */
                                control_task, "RT_task", RT_TASK_STACK_SIZE, SOEM_PORT_NAME, configMAX_PRIORITIES - 1,
                                rt_task_stack, &xTaskBuffer);

    vTaskStartScheduler();
    return 0;
}
