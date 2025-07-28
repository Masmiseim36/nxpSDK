/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include <stdint.h>
#include "fwk_debug_swo.h"

/* -------------------------------------------------------------------------- */
/*                              Macros                                        */
/* -------------------------------------------------------------------------- */

#define ITM_TCR_traceBusId_Position 16U         /* Defined to handle CMSIS version diversity */
#define CORESIGHT_ALLOW_W_VALUE     0xC5ACCE55U /* See ARM Coresight LAR Register bit assignments */

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void DBG_InitSWOFunnelMuxing(int funnel)
{
    volatile unsigned int reg_val = *(unsigned int *)0xE0044000U; /* Main Core Funnel PPB adress */

    /* Global enable for all DWT and ITM features */
    /* This enable debug TRACE clock and permit subsequent debug registers writes */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Write KEY value (0xC5ACCE55) to Lock Access Register to unlock the write access to ATB funnel by CPU. */
    (*(unsigned int *)0xE0044FB0U) = CORESIGHT_ALLOW_W_VALUE;
    /* As documented in the RM, there is a FUNNEL between the application core ITM output
     * and the NBU (radio) ITM output before the SWO pin output.
     * The FUNNEL registers are located on the PPB bus at 0xE0044000.
     * Configure CTRL_REG FUNNEL register to enable port 0 (application core) or port 1 (NBU).
     * Ports are controlled via ENS0/ENS1 bits. */
    reg_val &= 0xFFFFFFFCU; /* first, clear ENS0 & ENS1 */
    reg_val |= (unsigned int)funnel;
    /* Chose funnel depending on which core you want the SWO to apply on */
    *(unsigned int *)0xE0044000U = reg_val;
}

/* This function aims at configuring the SWO interface from SW code i.s.o. via a connected probe */
/* Function has to be called from Main Core and from NBU Core due to KW4x debug architecture     */
void DBG_SwInit_SWO(trace_swo_config_t *swoConfig)
{
    /* This computed prescaler value is used to derive SWO actual budrate from CPU core frequency */
    uint32_t prescaler = 0;

    /* Global enable for all DWT and ITM features */
    /* This enable debug TRACE clock and permit subsequent debug registers writes */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Clear SW lock for ITM registers */
#ifdef __CORE_CM3_H_GENERIC
    ITM->LAR = CORESIGHT_ALLOW_W_VALUE; //LAR offset 0xFB0
#else
    ITM->RESERVED6[43] = CORESIGHT_ALLOW_W_VALUE; //LAR offset 0xFB0
#endif

    /* ------------- */
    /* DISABLE ITM   */
    /* ------------- */
    /* Initially disable ITM and stimulus port. */
    /* To make sure that nothing is transferred via SWO when changing the SWO prescaler etc. */
    /* Disable used itmPort */
    ITM->TER &= ~(1UL << swoConfig->itmPort); /* Trace Enable Register */
    /* Disable ITM */
    ITM->TCR = 0U; /* Trace Control Register */

    /* -------------------*/
    /* CONFIGURE TPIU/SWO */
    /* -------------------*/
    /* Define which physical protocol we want to use on the SWO line */
    TPIU->SPPR = (uint32_t)swoConfig->protocol; // Selected Pin Protocol Register
    /* Define SWO baudrate by setting clock prescaler */
    prescaler  = swoConfig->clockRate / swoConfig->baudRate - 1U;
    TPIU->ACPR = prescaler & 0xFFFFU;     // Asynchronous Clock Prescaler Register
    TPIU->FFCR &= ~TPIU_FFCR_EnFCont_Msk; // 0 = Continuous formatting disabled

    /* --------------*/
    /* ENABLE ITM    */
    /* --------------*/
    /* Allow unprivileged code access for all stimulus ports */
    ITM->TPR = 0U; // Trace Privilege Register
    /* ITM Trace Control Register Enable ITM and SWO */
    ITM->TCR = ITM_TCR_ITMENA_Msk | ITM_TCR_SWOENA_Msk | (swoConfig->traceId << ITM_TCR_traceBusId_Position);

    /* Enable the chosen stimulus port bit */
    ITM->TER = 1UL << swoConfig->itmPort; // Trace Enable Register

    /* ------------- */
    /* ENABLE DWT    */
    /* ------------- */
    /* DWT->CTRL = 0x400011FF; //0x400013FF; Periodic PC sample packets enabling */
    /* DWT->CTRL |= DWT_CTRL_EXCTRCENA_Msk;  Interrupt trace enabling */
    /* ITM->TCR |= ITM_TCR_DWTENA_Msk;       Allow DWT traces forwarding */
}

/* Sends a char over SWO via the provided ITM port stimulus */
void DBG_SWO_PrintChar(uint32_t ch, uint32_t itmPort)
{
    if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && /* ITM enabled */
        ((ITM->TER & (1UL << itmPort)) != 0UL))     /* ITM Port itmPort enabled */
    {
        while (ITM->PORT[itmPort].u32 == 0UL)
        {
            __NOP();
        }
        ITM->PORT[itmPort].u8 = (uint8_t)ch;
    }
}

/* Sends a double word over SWO via the provided ITM port stimulus */
void DBG_SWO_PrintDoubleWord(uint32_t dw, uint32_t itmPort)
{
    if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && /* ITM enabled */
        ((ITM->TER & (1UL << itmPort)) != 0UL))     /* ITM Port itmPort enabled */
    {
        while (ITM->PORT[itmPort].u32 == 0UL)
        {
            __NOP();
        }
        ITM->PORT[itmPort].u32 = dw;
    }
}

/* Sends a double word over SWO on ITM port stimulus 0 */
/* No checks on ITM enables for speed optimization */
void DBG_SWO_PrintDoubleWordStim0(uint32_t dw)
{
    while (ITM->PORT[0].u32 == 0UL)
    {
        __NOP();
    }
    ITM->PORT[0].u32 = dw;
}
