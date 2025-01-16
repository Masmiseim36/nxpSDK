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

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_enet.h"
#include "fsl_debug_console.h"

#include "nicdrv.h"
#include "ethercattype.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "enet/soem_enet.h"
#include "enet/enet.h"
#include "soem_port.h"
#include "FreeRTOS.h"
#include "task.h"

#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NUM_1M (1000000UL)

#define SOEM_PORT_NAME "enet0"
#define EC_TIMEOUTMON  500

#define SOEM_PERIOD        125 /* 125us */
#define RT_TASK_STACK_SIZE 1024

#define ENET_RXBD_NUM (4)
#define ENET_TXBD_NUM (4)

#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)

static struct enet_if_port if_port;

static uint32_t timer_irq_period = 0; /* unit: microsecond*/

volatile struct timeval system_time_base = {.tv_sec = 0, .tv_usec = 0};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);

/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
    ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
    ENET_BUFF_ALIGNMENT);

static enet_buffer_config_t buffConfig[] = {{
    ENET_RXBD_NUM,
    ENET_TXBD_NUM,
    SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
    SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
    &g_rxBuffDescrip[0],
    &g_txBuffDescrip[0],
    &g_rxDataBuff[0][0],
    &g_txDataBuff[0][0],
    true,
    true,
    NULL,
}};

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

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F
     * Store immediate overlapping
     * exception return operation might vector to incorrect interrupt
     */
    SDK_ISR_EXIT_BARRIER;
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

/* OSHW: register enet port to SOEM stack */
static int if_port_init(void)
{
    struct soem_if_port soem_port;
    memset(&if_port, 0, sizeof(if_port));
    if_port.bufferConfig = buffConfig;
    if_port.base         = ENET;
    /* The miiMode should be set according to the different PHY interfaces. */
    if_port.mii_mode                   = kENET_RmiiMode;
    if_port.phy_config.autoNeg         = true;
    if_port.phy_config.phyAddr         = EXAMPLE_PHY_ADDRESS;
    if_port.phy_config.resource        = EXAMPLE_PHY_RESOURCE;
    if_port.phy_config.ops             = EXAMPLE_PHY_OPS;
    if_port.srcClock_Hz                = EXAMPLE_CLOCK_FREQ;
    if_port.phy_autonego_timeout_count = PHY_AUTONEGO_TIMEOUT_COUNT;
    if_port.phy_stability_delay_us     = PHY_STABILITY_DELAY_US;
    
    soem_port.port_init = enet_init;
    soem_port.port_send = enet_send;
    soem_port.port_recv = enet_recv;
    soem_port.port_link_status = enet_link_status;
    soem_port.port_close = enet_close;
    strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
    strncpy(soem_port.dev_name, "enet", SOEM_DEV_NAME_MAXLEN);
    soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
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
    } else {
        PRINTF("ec_init on %s failed.\r\n", ifname);
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
    if_port_init();
    rt_task = xTaskCreateStatic(/* The function that implements the task. */
                                control_task, "RT_task", RT_TASK_STACK_SIZE, SOEM_PORT_NAME, configMAX_PRIORITIES - 1,
                                rt_task_stack, &xTaskBuffer);

    vTaskStartScheduler();
    for (;;)
        ;
    return 0;
}
