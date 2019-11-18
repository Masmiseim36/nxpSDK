/*
 * Copyright (c) 2017 NXP Semiconductor, Inc.
 * Copyright 2017-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "rpmsg_platform.h"
#include "rpmsg_env.h"

#include "fsl_device_registers.h"
#include "fsl_mu.h"
#include "fsl_irqsteer.h"

#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
#error "This RPMsg-Lite port requires RL_USE_ENVIRONMENT_CONTEXT set to 0"
#endif

#define APP_MU_IRQ_PRIORITY (3U)

/* The MU instance used for CM4 and A core communication */
#if defined(MIMX8QM_CM4_CORE0)
#define APP_M4_A_MU LSIO__MU5_B
#define APP_M4_A_MU_IRQn LSIO_MU5_INT_B_IRQn
#elif defined(MIMX8QM_CM4_CORE1)
#define APP_M4_A_MU LSIO__MU6_B
#define APP_M4_A_MU_IRQn LSIO_MU6_INT_B_IRQn
#else
#error "Unsupported CPU core!"
#endif

/* The MU instance used for the communication between two CM4 cores */
/*
 -------------------------------------------------------
   Platform   |  M4_0 PT    |  M4_1 PT     | A core PT
  i.MX8QM/QX  | LSIO MU5_B  |       -      | LSIO MU5_A
  i.MX8QM     |      -      |  LSIO MU6_B  | LSIO MU6_A
  i.MX8QM     | LSIO MU7_A  |  LSIO MU7_B  |  -
 -------------------------------------------------------
*/
#if defined(MIMX8QM_CM4_CORE0)
#define APP_M4_M4_MU LSIO__MU7_A
#define APP_M4_M4_MU_IRQn LSIO_MU7_INT_A_IRQn
#elif defined(MIMX8QM_CM4_CORE1)
#define APP_M4_M4_MU LSIO__MU7_B
#define APP_M4_M4_MU_IRQn LSIO_MU7_INT_B_IRQn
#endif

/* Get the NVIC IRQn of given IRQSTEER IRQn */
#define GET_IRQSTEER_MASTER_IRQn(IRQn) (IRQn_Type)(IRQSTEER_0_IRQn + (IRQn - FSL_FEATURE_IRQSTEER_IRQ_START_INDEX) / 64U)

static int isr_counter0 = 0; /* RL_PLATFORM_IMX8QM_M4_A_USER_LINK_ID isr counter */
static int isr_counter1 = 0; /* RL_PLATFORM_IMX8QM_M4_M4_USER_LINK_ID isr counter */
static int disable_counter0 = 0;
static int disable_counter1 = 0;
static void *lock;

int platform_init_interrupt(int vq_id, void *isr_data)
{
    /* Register ISR to environment layer */
    env_register_isr(vq_id, isr_data);

    /* Prepare the MU Hardware, enable channel 1 interrupt */
    env_lock_mutex(lock);

    switch (RL_GET_COM_ID(vq_id))
    {
        case RL_PLATFORM_IMX8QM_M4_A_COM_ID:
            assert(0 <= isr_counter0);
            if (!isr_counter0)
            {
                MU_EnableInterrupts(APP_M4_A_MU, (1U << 27U) >> RPMSG_MU_CHANNEL);
            }
            isr_counter0++;
            break;
        case RL_PLATFORM_IMX8QM_M4_M4_COM_ID:
            assert(0 <= isr_counter1);
            if (!isr_counter1)
            {
                MU_EnableInterrupts(APP_M4_M4_MU, (1U << 27U) >> RPMSG_MU_CHANNEL);
            }
            isr_counter1++;
            break;
        default:
            break;
    }

    env_unlock_mutex(lock);

    return 0;
}

int platform_deinit_interrupt(int vq_id)
{
    /* Prepare the MU Hardware */
    env_lock_mutex(lock);

    switch (RL_GET_COM_ID(vq_id))
    {
        case RL_PLATFORM_IMX8QM_M4_A_COM_ID:
            assert(0 < isr_counter0);
            isr_counter0--;
            if (!isr_counter0)
            {
                MU_DisableInterrupts(APP_M4_A_MU, (1U << 27U) >> RPMSG_MU_CHANNEL);
            }
            break;
        case RL_PLATFORM_IMX8QM_M4_M4_COM_ID:
            assert(0 < isr_counter1);
            isr_counter1--;
            if (!isr_counter1)
            {
                MU_DisableInterrupts(APP_M4_M4_MU, (1U << 27U) >> RPMSG_MU_CHANNEL);
            }
            break;
        default:
            break;
    }

    /* Unregister ISR from environment layer */
    env_unregister_isr(vq_id);

    env_unlock_mutex(lock);

    return 0;
}

void platform_notify(int vq_id)
{
    /* Only vring id and queue id is needed in msg */
    uint32_t msg = RL_GEN_MU_MSG(vq_id);

    env_lock_mutex(lock);
    /* As Linux suggests, use MU->Data Channel 1 as communication channel */
    switch (RL_GET_COM_ID(vq_id))
    {
        case RL_PLATFORM_IMX8QM_M4_A_COM_ID:
            MU_SendMsg(APP_M4_A_MU, RPMSG_MU_CHANNEL, msg);
            break;
        case RL_PLATFORM_IMX8QM_M4_M4_COM_ID:
            MU_SendMsg(APP_M4_M4_MU, RPMSG_MU_CHANNEL, msg);
            break;
        default:
            break;
    }
    env_unlock_mutex(lock);
}

/*
 * MU Interrrupt RPMsg handler
 */
#if defined(MIMX8QM_CM4_CORE0)
int LSIO_MU5_INT_B_IRQHandler(void)
{
    uint32_t channel;

    if (((1U << 27U) >> RPMSG_MU_CHANNEL) & MU_GetStatusFlags(APP_M4_A_MU))
    {
        channel = MU_ReceiveMsgNonBlocking(APP_M4_A_MU, RPMSG_MU_CHANNEL); /* Read message from RX register. */
        env_isr((channel >> 16) | (RL_PLATFORM_IMX8QM_M4_A_COM_ID << 3));
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif

    return 0;
}

#elif defined(MIMX8QM_CM4_CORE1)
int LSIO_MU6_INT_B_IRQHandler(void)
{
    uint32_t channel;

    if (((1U << 27U) >> RPMSG_MU_CHANNEL) & MU_GetStatusFlags(APP_M4_A_MU))
    {
        channel = MU_ReceiveMsgNonBlocking(APP_M4_A_MU, RPMSG_MU_CHANNEL); /* Read message from RX register. */
        env_isr((channel >> 16) | (RL_PLATFORM_IMX8QM_M4_A_COM_ID << 3));
    }

/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
    return 0;
}
#endif

#if defined(MIMX8QM_CM4_CORE0)
int LSIO_MU7_INT_A_IRQHandler(void)
{
    uint32_t channel;

    if (((1U << 27U) >> RPMSG_MU_CHANNEL) & MU_GetStatusFlags(APP_M4_M4_MU))
    {
        channel = MU_ReceiveMsgNonBlocking(APP_M4_M4_MU, RPMSG_MU_CHANNEL); /* Read message from RX register. */
        env_isr((channel >> 16) | (RL_PLATFORM_IMX8QM_M4_M4_COM_ID << 3));
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif

    return 0;
}
#elif defined(MIMX8QM_CM4_CORE1)
int LSIO_MU7_INT_B_IRQHandler(void)
{
    uint32_t channel;

    if (((1U << 27U) >> RPMSG_MU_CHANNEL) & MU_GetStatusFlags(APP_M4_M4_MU))
    {
        channel = MU_ReceiveMsgNonBlocking(APP_M4_M4_MU, RPMSG_MU_CHANNEL); /* Read message from RX register. */
        env_isr((channel >> 16) | (RL_PLATFORM_IMX8QM_M4_M4_COM_ID << 3));
    }

/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif

    return 0;
}
#endif
/**
 * platform_time_delay
 *
 * @param num_msec Delay time in ms.
 *
 * This is not an accurate delay, it ensures at least num_msec passed when return.
 */
void platform_time_delay(int num_msec)
{
    uint32_t loop;

    /* Recalculate the CPU frequency */
    SystemCoreClockUpdate();

    /* Calculate the CPU loops to delay, each loop has 3 cycles */
    loop = SystemCoreClock / 3 / 1000 * num_msec;

    /* There's some difference among toolchains, 3 or 4 cycles each loop */
    while (loop)
    {
        __NOP();
        loop--;
    }
}

/**
 * platform_in_isr
 *
 * Return whether CPU is processing IRQ
 *
 * @return True for IRQ, false otherwise.
 *
 */
int platform_in_isr(void)
{
    return ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0);
}

/**
 * platform_interrupt_enable
 *
 * Enable peripheral-related interrupt with passed priority and type.
 *
 * @param vq_id Vector ID that need to be converted to IRQ number
 *
 * @return vq_id. Return value is never checked..
 *
 */
int platform_interrupt_enable(unsigned int vq_id)
{
    __asm volatile("cpsid i");

    switch (RL_GET_COM_ID(vq_id))
    {
        case RL_PLATFORM_IMX8QM_M4_A_COM_ID:
            assert(0 < disable_counter0);
            disable_counter0--;
            if (!disable_counter0)
            {
                NVIC_EnableIRQ(GET_IRQSTEER_MASTER_IRQn(APP_M4_A_MU_IRQn));
            }
            break;
        case RL_PLATFORM_IMX8QM_M4_M4_COM_ID:
            assert(0 < disable_counter1);
            disable_counter1--;
            if (!disable_counter1)
            {
                NVIC_EnableIRQ(GET_IRQSTEER_MASTER_IRQn(APP_M4_M4_MU_IRQn));
            }
            break;
        default:
            break;
    }
    __asm volatile("cpsie i");
    return (vq_id);
}

/**
 * platform_interrupt_disable
 *
 * Disable peripheral-related interrupt.
 *
 * @param vq_id Vector ID that need to be converted to IRQ number
 *
 * @return vq_id. Return value is never checked.
 *
 */
int platform_interrupt_disable(unsigned int vq_id)
{
    __asm volatile("cpsid i");
    // virtqueues use the same NVIC vector
    // if counter is set - the interrupts are disabled
    switch (RL_GET_COM_ID(vq_id))
    {
        case RL_PLATFORM_IMX8QM_M4_A_COM_ID:
            assert(0 <= disable_counter0);
            if (!disable_counter0)
            {
                NVIC_DisableIRQ(GET_IRQSTEER_MASTER_IRQn(APP_M4_A_MU_IRQn));
            }
            disable_counter0++;
            break;
        case RL_PLATFORM_IMX8QM_M4_M4_COM_ID:
            assert(0 <= disable_counter1);
            if (!disable_counter1)
            {
                NVIC_DisableIRQ(GET_IRQSTEER_MASTER_IRQn(APP_M4_M4_MU_IRQn));
            }
            disable_counter1++;
            break;
        default:
            break;
    }

    __asm volatile("cpsie i");
    return (vq_id);
}

/**
 * platform_map_mem_region
 *
 * Dummy implementation
 *
 */
void platform_map_mem_region(unsigned int vrt_addr, unsigned int phy_addr, unsigned int size, unsigned int flags)
{
}

/**
 * platform_cache_all_flush_invalidate
 *
 * Dummy implementation
 *
 */
void platform_cache_all_flush_invalidate(void)
{
}

/**
 * platform_cache_disable
 *
 * Dummy implementation
 *
 */
void platform_cache_disable(void)
{
}

/**
 * platform_vatopa
 *
 * Dummy implementation
 *
 */
unsigned long platform_vatopa(void *addr)
{
    return ((unsigned long)addr);
}

/**
 * platform_patova
 *
 * Dummy implementation
 *
 */
void *platform_patova(unsigned long addr)
{
    return ((void *)addr);
}

/**
 * platform_init
 *
 * platform/environment init
 */
int platform_init(void)
{
    /*
     * Prepare for the MU Interrupt
     *  MU must be initialized before rpmsg init is called
     */
    MU_Init(APP_M4_A_MU);
    NVIC_SetPriority(GET_IRQSTEER_MASTER_IRQn(APP_M4_A_MU_IRQn), APP_MU_IRQ_PRIORITY);
    NVIC_EnableIRQ(GET_IRQSTEER_MASTER_IRQn(APP_M4_A_MU_IRQn));
    IRQSTEER_EnableInterrupt(IRQSTEER, APP_M4_A_MU_IRQn);

   /* Prepare for the MU Interrupt for the MU used between two M4s*/
    MU_Init(APP_M4_M4_MU);
    NVIC_SetPriority(GET_IRQSTEER_MASTER_IRQn(APP_M4_M4_MU_IRQn), APP_MU_IRQ_PRIORITY);
    NVIC_EnableIRQ(GET_IRQSTEER_MASTER_IRQn(APP_M4_M4_MU_IRQn));
    IRQSTEER_EnableInterrupt(IRQSTEER, APP_M4_M4_MU_IRQn);

    /* Create lock used in multi-instanced RPMsg */
    if(0 != env_create_mutex(&lock, 1))
    {
        return -1;
    }

    return 0;
}

/**
 * platform_deinit
 *
 * platform/environment deinit process
 */
int platform_deinit(void)
{
    MU_Deinit(APP_M4_A_MU);
    IRQSTEER_DisableInterrupt(IRQSTEER, APP_M4_A_MU_IRQn);
    MU_Deinit(APP_M4_M4_MU);
    IRQSTEER_DisableInterrupt(IRQSTEER, APP_M4_M4_MU_IRQn);

    /* Delete lock used in multi-instanced RPMsg */
    env_delete_mutex(lock);
    lock = NULL;
    return 0;
}
