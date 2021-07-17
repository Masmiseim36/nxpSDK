/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifdef TFM_PARTITION_PROTECTED_STORAGE
#include "secure_fw/partitions/protected_storage/psa_manifest/tfm_protected_storage.h"
#endif /* TFM_PARTITION_PROTECTED_STORAGE */
#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
#include "secure_fw/partitions/internal_trusted_storage/psa_manifest/tfm_internal_trusted_storage.h"
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */
#ifdef TFM_PARTITION_AUDIT_LOG
#include "secure_fw/partitions/audit_logging/psa_manifest/tfm_audit_logging.h"
#endif /* TFM_PARTITION_AUDIT_LOG */
#ifdef TFM_PARTITION_CRYPTO
#include "secure_fw/partitions/crypto/psa_manifest/tfm_crypto.h"
#endif /* TFM_PARTITION_CRYPTO */
#ifdef TFM_PARTITION_PLATFORM
#include "secure_fw/partitions/platform/psa_manifest/tfm_platform.h"
#endif /* TFM_PARTITION_PLATFORM */
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
#include "secure_fw/partitions/initial_attestation/psa_manifest/tfm_initial_attestation.h"
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */
#ifdef TFM_PARTITION_TEST_CORE
#include "test_services\tfm_core_test\psa_manifest\tfm_ss_core_test.h"
#endif /* TFM_PARTITION_TEST_CORE */
#ifdef TFM_PARTITION_TEST_CORE
#include "test_services\tfm_core_test_2\psa_manifest\tfm_ss_core_test_2.h"
#endif /* TFM_PARTITION_TEST_CORE */
#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
#include "test_services\tfm_secure_client_service\psa_manifest\tfm_secure_client_service.h"
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */
#ifdef TFM_PARTITION_TEST_CORE_IPC
#include "test_services\tfm_ipc_service\psa_manifest\tfm_ipc_service_test.h"
#endif /* TFM_PARTITION_TEST_CORE_IPC */
#ifdef TFM_PARTITION_TEST_CORE_IPC
#include "test_services\tfm_ipc_client\psa_manifest\tfm_ipc_client_test.h"
#endif /* TFM_PARTITION_TEST_CORE_IPC */
#ifdef TFM_ENABLE_IRQ_TEST
#include "test_services\tfm_irq_test_service_1\psa_manifest\tfm_irq_test_service_1.h"
#endif /* TFM_ENABLE_IRQ_TEST */
#ifdef TFM_PARTITION_TEST_PS
#include "test_services\tfm_ps_test_service\psa_manifest\tfm_ps_test_service.h"
#endif /* TFM_PARTITION_TEST_PS */
#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
#include "test_services\tfm_secure_client_2\psa_manifest\tfm_secure_client_2.h"
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */
#ifdef TFM_PARTITION_PSA_PROXY
#include "secure_fw/partitions/psa_proxy/psa_manifest/tfm_psa_proxy.h"
#endif /* TFM_PARTITION_PSA_PROXY */
#ifdef TFM_PARTITION_FIRMWARE_UPDATE
#include "secure_fw/partitions/firmware_update/psa_manifest/tfm_firmware_update.h"
#endif /* TFM_PARTITION_FIRMWARE_UPDATE */
#ifdef TFM_PARTITION_FFM11
#include "secure_fw/partitions/tfm_ffm11_partition/psa_manifest/tfm_ffm11_partition.h"
#endif /* TFM_PARTITION_FFM11 */
#include "psa_manifest/pid.h"

/* Definitions of the signals of the IRQs */
const struct tfm_core_irq_signal_data_t tfm_core_irq_signals[] = {
#ifdef TFM_ENABLE_IRQ_TEST
    { TFM_IRQ_TEST_1, SPM_CORE_IRQ_TEST_1_SIGNAL_TIMER_0_IRQ, TFM_TIMER0_IRQ, 64 },
#endif /* TFM_ENABLE_IRQ_TEST */
   {0, 0, 0, 0}                         /* add dummy element to avoid non-standard empty array */
};

const size_t tfm_core_irq_signals_count = (sizeof(tfm_core_irq_signals) /
                                           sizeof(*tfm_core_irq_signals)) - 1;  /* adjust for the dummy element */

extern void priv_irq_handler_main(uint32_t partition_id,
                                  uint32_t unpriv_handler,
                                  uint32_t irq_signal,
                                  uint32_t irq_line);

/* Forward declarations of unpriv IRQ handlers*/
#ifdef TFM_ENABLE_IRQ_TEST
extern void SPM_CORE_IRQ_TEST_1_SIGNAL_TIMER_0_IRQ_isr(void);
#endif /* TFM_ENABLE_IRQ_TEST */


/* Definitions of privileged IRQ handlers */
#ifdef TFM_ENABLE_IRQ_TEST
void TFM_TIMER0_IRQ_Handler(void)
{
    priv_irq_handler_main(TFM_IRQ_TEST_1,
                          (uint32_t)SPM_CORE_IRQ_TEST_1_SIGNAL_TIMER_0_IRQ_isr,
                          SPM_CORE_IRQ_TEST_1_SIGNAL_TIMER_0_IRQ,
                          TFM_TIMER0_IRQ);
}

#endif /* TFM_ENABLE_IRQ_TEST */

