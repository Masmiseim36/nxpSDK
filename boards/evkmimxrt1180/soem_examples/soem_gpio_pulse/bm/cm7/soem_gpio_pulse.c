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

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyrtl8201.h"
#include "fsl_msgintr.h"
#include "fsl_gpt.h"

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "soem_port.h"
#include "netc_ep/soem_netc_ep.h"
#include "netc_ep/netc_ep.h"

#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
#define EP0_PORT  0x00U

#define EP0_PHY_ADDR       0x03U
#define NETC_FREQ          CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

#define EP_RING_NUM          3U
#define EP_RXBD_NUM          8U
#define EP_TXBD_NUM          8U
#define EP_BD_ALIGN          128U
#define EP_BUFF_SIZE_ALIGN   64U
#define EP_RXBUFF_SIZE       1518U
#define EP_TXBUFF_SIZE       1518U
#define EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EP_RXBUFF_SIZE, EP_BUFF_SIZE_ALIGN)

#define EP_TXFRAME_NUM 20U
#define TX_INTR_MSG_DATA  1U
#define RX_INTR_MSG_DATA  2U
#define TX_MSIX_ENTRY_IDX 0U
#define RX_MSIX_ENTRY_IDX 1U

#define NUM_1M      (1000000UL)
#define SOEM_PERIOD 125 /* 125 us */

#define SOEM_PORT_NAME "EtherCAT0"

/*******************************************************************************
 * Variables
 ******************************************************************************/

static struct netc_ep_if_port if_port;

static uint32_t timer_irq_period = 0; /* unit: microsecond*/

volatile struct timeval system_time_base = {.tv_sec = 0, .tv_usec = 0};

/* Buffer descriptor and buffer memeory. */
typedef uint8_t rx_buffer_t[EP_RXBUFF_SIZE_ALIGN];

AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EP_RING_NUM][EP_RXBD_NUM], EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EP_RING_NUM][EP_TXBD_NUM], EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EP_RING_NUM][EP_RXBD_NUM], EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EP_TXBUFF_SIZE], EP_BUFF_SIZE_ALIGN);
uint64_t rxBuffAddrArray[EP_RING_NUM][EP_RXBD_NUM];

static netc_tx_frame_info_t g_txDirty[EP_RING_NUM][EP_TXBD_NUM];
//static netc_tx_frame_info_t txFrameInfo = {0};
//static volatile bool txOver;

static char IOmap[100];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

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
        if_port.txOver = true;
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(&if_port.g_ep_handle, 1);
    }
}

//static status_t ReclaimCallback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
//{
//    txFrameInfo = *frameInfo;
//    return kStatus_Success;
//}

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

static netc_rx_bdr_config_t rxBdrConfig = {0};
static netc_tx_bdr_config_t txBdrConfig = {0};
static netc_bdr_config_t bdrConfig;
static netc_msix_entry_t msixEntry[2];
static ep_config_t ep_config;

/* OSHW: register enet port to SOEM stack */
static int if_port_init(void)
{
	struct soem_if_port soem_port;

    bdrConfig.rxBdrConfig = &rxBdrConfig;
	bdrConfig.txBdrConfig = &txBdrConfig;
    uint32_t msgAddr;

    PRINTF("\r\nNETC EP frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(MSGINTR1, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(MSGINTR1, 0);
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

void irq_wake_task(void)
{
    return;
}

void control_task(char *ifname)
{
    int oloop, iloop;
    int expectedWKC;
    volatile int wkc;
    int old_switch0 = 0;
    int old_switch1 = 0;
    struct timeval current_time;
    struct timeval sleep_time;
    struct timeval target_time;
    struct timeval last_time;
    struct timeval diff_time;
    sleep_time.tv_sec  = 0;
    sleep_time.tv_usec = 125;

    /* initialise SOEM, and if_port */
    if (ec_init(ifname))
    {
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
            char chk = 40;
            /* wait for all slaves to reach OP state */
            do
            {
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            } while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
            PRINTF("Operational state reached for all slaves.\r\n");
            /* cyclic loop */
            int is_expired;
            ec_send_processdata();
            osal_gettime(&last_time);
            while (1)
            {
                osal_gettime(&current_time);
                timeradd(&current_time, &sleep_time, &target_time);
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
                osal_gettime(&current_time);
                timersub(&current_time, &last_time, &diff_time);
                is_expired = timercmp(&target_time, &current_time, <=);
                if (diff_time.tv_usec > 130)
                {
                    last_time.tv_usec = current_time.tv_usec;
                }
                last_time.tv_usec = current_time.tv_usec;
                last_time.tv_sec  = current_time.tv_sec;
                if (!is_expired)
                {
                    timersub(&target_time, &current_time, &sleep_time);
                    ec_send_processdata();
                    osal_usleep(sleep_time.tv_usec);
                    sleep_time.tv_usec = SOEM_PERIOD;
                }
                else
                {
                    while (1)
                        ;
                }
            }
        }
    }
}


status_t NETC_EP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = NETC_FREQ,
    };

    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&if_port.s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

static status_t NETC_EP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&if_port.s_emdio_handle, phyAddr, regAddr, data);
}

static status_t NETC_EP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&if_port.s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t NETC_EP_Phy8201SetUp(phy_handle_t *handle)
{
    status_t result;
    uint16_t data;

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 7);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Read(handle, 16, &data);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* CRS/DV pin is RXDV signal. */
    data |= (1U << 2);
    result = PHY_Write(handle, 16, data);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 0);

    return result;
}

phy_rtl8201_resource_t s_phy_resource;
status_t NETC_EP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phy8201Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8201_ops,
    };
    rgpio_pin_config_t pinConfig = {.pinDirection = kRGPIO_DigitalOutput, .outputLogic = 0};

    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    /* Reset PHY8211 for ETH1(Switch port1). Reset 10ms, wait 72ms. */
    RGPIO_PinInit(RGPIO4, 13, &pinConfig);
    RGPIO_PinInit(RGPIO4, 25, &pinConfig);
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    RGPIO_PinWrite(RGPIO4, 13, 1);
    RGPIO_PinWrite(RGPIO4, 25, 1);
    SDK_DelayAtLeastUs(150000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY for EP. */
    s_phy_resource.write = NETC_EP_EMDIOWrite;
    s_phy_resource.read  = NETC_EP_EMDIORead;
    
	phy8201Config.resource = &s_phy_resource;
    phy8201Config.phyAddr  = EP0_PHY_ADDR;
    result                 = PHY_Init(&if_port.phy_handle, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = NETC_EP_Phy8201SetUp(&if_port.phy_handle);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}


/*!
 * @brief Main function
 */
int main(void)
{
    bool link;
    status_t result = kStatus_Success;
    BOARD_InitHardware();

    result = NETC_EP_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = NETC_EP_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    do
    {
		PHY_GetLinkStatus(&if_port.phy_handle, &link);
    } while (!link);

    PRINTF("Start the soem_gpio_pulse baremetal example...\r\n");

    osal_timer_init(NUM_1M, 0);
    if_port_init();
    control_task(SOEM_PORT_NAME);
    return 0;
}
