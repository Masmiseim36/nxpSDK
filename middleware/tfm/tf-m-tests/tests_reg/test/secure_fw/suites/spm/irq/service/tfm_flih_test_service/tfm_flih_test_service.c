/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis_compiler.h"
#include "tfm_plat_test.h"
#include "spm_test_defs.h"
#include "tfm_sp_log.h"
#include "psa/service.h"
#include "psa_manifest/tfm_flih_test_service.h"

/* The execution flow ensures there are no race conditions for test_type */
static int32_t test_type = TFM_FLIH_TEST_CASE_INVALID;
/*
 * Records times of triggered
 *
 * The test cases do not care about exact value of flih_timer_triggered.
 * They only needs to know if it has reached a certain value.
 * And it is a single-read-single-writer model.
 * So the race condition of accessing flih_timer_triggered between the Partition
 * thread and IRS is acceptable.
 */
static volatile uint32_t flih_timer_triggered = 0;

static psa_flih_result_t tfm_flih_test_timer_handler(void)
{
    tfm_plat_test_secure_timer_clear_intr();

    switch (test_type) {
    case TFM_FLIH_TEST_CASE_1:
        flih_timer_triggered += 1;
        return PSA_FLIH_NO_SIGNAL;
    case TFM_FLIH_TEST_CASE_2:
        flih_timer_triggered += 1;
        if (flih_timer_triggered == 10) {
            return PSA_FLIH_SIGNAL;
        } else {
            return PSA_FLIH_NO_SIGNAL;
        }
        break;
    default:
        psa_panic();
        break;
    }

    return PSA_FLIH_NO_SIGNAL;
}

static void flih_test_case_1(const psa_msg_t *msg, psa_signal_t timer_irq_signal)
{
    test_type = msg->type;

    flih_timer_triggered = 0;

    psa_irq_enable(timer_irq_signal);

    tfm_plat_test_secure_timer_start();

    while (flih_timer_triggered < 10) {
        __WFI();
    }

    tfm_plat_test_secure_timer_stop();

    psa_irq_disable(timer_irq_signal);
}

static void flih_test_case_2(const psa_msg_t *msg, psa_signal_t timer_irq_signal)
{
    test_type = msg->type;

    flih_timer_triggered = 0;

    psa_irq_enable(timer_irq_signal);

    tfm_plat_test_secure_timer_start();

    if (psa_wait(timer_irq_signal, PSA_BLOCK) != timer_irq_signal) {
        psa_panic();
    }
    tfm_plat_test_secure_timer_stop();

    psa_reset_signal(timer_irq_signal);
    psa_irq_disable(timer_irq_signal);
}

psa_flih_result_t tfm_timer0_irq_flih(void)
{
    return tfm_flih_test_timer_handler();
}

static psa_status_t flih_test_handle_msg(const psa_msg_t *msg)
{
    psa_status_t status;

    switch (msg->type) {
    case TFM_FLIH_TEST_CASE_1:
        flih_test_case_1(msg, TFM_TIMER0_IRQ_SIGNAL);
        status = PSA_SUCCESS;
        break;
    case TFM_FLIH_TEST_CASE_2:
        flih_test_case_2(msg, TFM_TIMER0_IRQ_SIGNAL);
        status = PSA_SUCCESS;
        break;
    default:
        LOG_ERRFMT("FLIH test service: Invalid message type: 0x%x\r\n",
            msg->type);
        status = PSA_ERROR_PROGRAMMER_ERROR;
        break;
    }

    return status;
}

#if TFM_SP_FLIH_TEST_MODEL_IPC == 1
static void flih_test_get_msg(psa_signal_t signal, psa_msg_t *msg)
{
    psa_status_t status;

    status = psa_get(signal, msg);
    if (status != PSA_SUCCESS) {
        psa_panic();
    }
}

void tfm_flih_test_service_entry(void)
{
    psa_signal_t signals = 0;
    psa_msg_t msg;

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_FLIH_TEST_CASE_SIGNAL) {
            flih_test_get_msg(TFM_FLIH_TEST_CASE_SIGNAL, &msg);
            psa_reply(msg.handle, flih_test_handle_msg(&msg));
        }
    }
}
#elif TFM_SP_FLIH_TEST_MODEL_SFN == 1
psa_status_t tfm_flih_test_case_sfn(const psa_msg_t *msg)
{
    return flih_test_handle_msg(msg);
}
#endif
