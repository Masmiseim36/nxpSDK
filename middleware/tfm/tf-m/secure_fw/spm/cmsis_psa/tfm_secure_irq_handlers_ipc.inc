/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

/* Definitions of privileged IRQ handlers (if any) */
#ifdef TEST_NS_SLIH_IRQ
#include "../tf-m-tests/test/test_services/tfm_slih_test_service/psa_manifest/tfm_slih_test_service.h"
extern struct partition_tfm_sp_slih_test_load_info_t tfm_sp_slih_test_load;
void TFM_TIMER0_IRQ_Handler(void)
{
    spm_interrupt_handler((struct partition_load_info_t *)&tfm_sp_slih_test_load, TFM_TIMER0_IRQ_SIGNAL, TFM_TIMER0_IRQ, NULL);
}
#endif /* TEST_NS_SLIH_IRQ */

#ifdef TEST_NS_FLIH_IRQ
#include "../tf-m-tests/test/test_services/tfm_flih_test_service/psa_manifest/tfm_flih_test_service.h"
extern struct partition_tfm_sp_flih_test_load_info_t tfm_sp_flih_test_load;
void TFM_TIMER0_IRQ_Handler(void)
{
    spm_interrupt_handler((struct partition_load_info_t *)&tfm_sp_flih_test_load, TFM_TIMER0_IRQ_SIGNAL, TFM_TIMER0_IRQ, tfm_timer0_irq_flih);
}
#endif /* TEST_NS_FLIH_IRQ */

