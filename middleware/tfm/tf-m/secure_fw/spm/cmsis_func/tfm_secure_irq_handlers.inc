/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#include "psa_manifest/pid.h"

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX  //NXP
#error "Framework version greater than 1.0 is not supported"
#define                                                         (1U << (31))
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP
#ifdef TEST_NS_SLIH_IRQ  //NXP
#error "Framework version greater than 1.0 is not supported"
#define                                                         (1U << (31))
#endif /* TEST_NS_SLIH_IRQ */ //NXP
#ifdef TEST_NS_FLIH_IRQ  //NXP
#error "Framework version greater than 1.0 is not supported"
#define                                                         (1U << (31))
#endif /* TEST_NS_FLIH_IRQ */ //NXP

/* Definitions of the signals of the IRQs */
const struct tfm_core_irq_signal_data_t tfm_core_irq_signals[] = {
#ifdef TFM_PARTITION_NS_AGENT_MAILBOX   //NXP
    { TFM_NS_MAILBOX_AGENT, , MAILBOX_IRQ },
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP
#ifdef TEST_NS_SLIH_IRQ   //NXP
    { TFM_SP_SLIH_TEST, , TFM_TIMER0_IRQ },
#endif /* TEST_NS_SLIH_IRQ */ //NXP
#ifdef TEST_NS_FLIH_IRQ   //NXP
    { TFM_SP_FLIH_TEST, , TFM_TIMER0_IRQ },
#endif /* TEST_NS_FLIH_IRQ */ //NXP
   {0,0,0}                         /* add dummy element to avoid non-standard empty array */
};

const size_t tfm_core_irq_signals_count = (sizeof(tfm_core_irq_signals) /
                                           sizeof(*tfm_core_irq_signals)) - 1;  /* adjust for the dummy element */

extern void priv_irq_handler_main(uint32_t partition_id,
                                  uint32_t unpriv_handler,
                                  uint32_t irq_signal,
                                  uint32_t irq_line);

/* Forward declarations of unpriv IRQ handlers*/
#ifdef TFM_PARTITION_NS_AGENT_MAILBOX ///NXP
extern void _isr(void);
#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TEST_NS_SLIH_IRQ ///NXP
extern void _isr(void);
#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ ///NXP
extern void _isr(void);
#endif /* TEST_NS_FLIH_IRQ */ //NXP


/* Definitions of privileged IRQ handlers */
#ifdef TFM_PARTITION_NS_AGENT_MAILBOX //NXP
void MAILBOX_IRQ_Handler(void)
{
    priv_irq_handler_main(TFM_NS_MAILBOX_AGENT,
                          (uint32_t)_isr,
                          ,
                          MAILBOX_IRQ);
}

#endif /* TFM_PARTITION_NS_AGENT_MAILBOX */ //NXP

#ifdef TEST_NS_SLIH_IRQ //NXP
void TFM_TIMER0_IRQ_Handler(void)
{
    priv_irq_handler_main(TFM_SP_SLIH_TEST,
                          (uint32_t)_isr,
                          ,
                          TFM_TIMER0_IRQ);
}

#endif /* TEST_NS_SLIH_IRQ */ //NXP

#ifdef TEST_NS_FLIH_IRQ //NXP
void TFM_TIMER0_IRQ_Handler(void)
{
    priv_irq_handler_main(TFM_SP_FLIH_TEST,
                          (uint32_t)_isr,
                          ,
                          TFM_TIMER0_IRQ);
}

#endif /* TEST_NS_FLIH_IRQ */ //NXP

