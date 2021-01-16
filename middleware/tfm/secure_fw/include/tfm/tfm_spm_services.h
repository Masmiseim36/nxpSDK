/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPM_SERVICES_H__
#define __TFM_SPM_SERVICES_H__

#include "psa/service.h"

enum tfm_spm_request_type_t {
    TFM_SPM_REQUEST_RESET_VOTE,
};

/**
 * \brief Request a vote from SPM on a system reset
 *
 * \return Returns 0 if request is accepted, any other value means reject
 */
int32_t tfm_spm_request_reset_vote(void);

void tfm_enable_irq(psa_signal_t irq_signal);
void tfm_disable_irq(psa_signal_t irq_signal);

#endif /* __TFM_SPM_SERVICES_H__ */
