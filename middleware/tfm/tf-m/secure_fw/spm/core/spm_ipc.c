/*
 * Copyright (c) 2018-2023, Arm Limited. All rights reserved.
 * Copyright (c) 2021-2023 Cypress Semiconductor Corporation (an Infineon
 * company) or an affiliate of Cypress Semiconductor Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include "bitops.h"
#include "config_impl.h"
#include "config_spm.h"
#include "critical_section.h"
#include "current.h"
#include "fih.h"
#include "psa/client.h"
#include "psa/service.h"
#include "thread.h"
#include "internal_status_code.h"
#include "tfm_api.h"
#include "tfm_arch.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_interrupt.h"
#include "tfm_hal_isolation.h"
#include "spm.h"
#include "tfm_peripherals_def.h"
#include "tfm_nspm.h"
#include "tfm_rpc.h"
#include "tfm_core_trustzone.h"
#include "lists.h"
#include "tfm_pools.h"
#include "region.h"
#include "psa_manifest/pid.h"
#include "ffm/backend.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/asset_defs.h"
#include "load/spm_load_api.h"
#include "tfm_nspm.h"

/* Partition and service runtime data list head/runtime data table */
static struct service_head_t services_listhead;
struct service_t *stateless_services_ref_tbl[STATIC_HANDLE_NUM_LIMIT];

/* Partition management functions */

/* This API is only used in IPC backend. */
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
struct connection_t *spm_get_handle_by_signal(struct partition_t *p_ptn,
                                              psa_signal_t signal)
{
    struct connection_t *p_handle_iter;
    struct connection_t **pr_handle_iter, **last_found_handle_holder = NULL;
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;
    uint32_t nr_found_msgs = 0;

    CRITICAL_SECTION_ENTER(cs_assert);

    /* Return the last found message which applies a FIFO mechanism. */
    UNI_LIST_FOREACH_NODE_PNODE(pr_handle_iter, p_handle_iter,
                                p_ptn, p_handles) {
        if (p_handle_iter->service->p_ldinf->signal == signal) {
            last_found_handle_holder = pr_handle_iter;
            nr_found_msgs++;
        }
    }

    if (last_found_handle_holder) {
        p_handle_iter = *last_found_handle_holder;
        UNI_LIST_REMOVE_NODE_BY_PNODE(last_found_handle_holder, p_handles);

        if (nr_found_msgs == 1) {
            p_ptn->signals_asserted &= ~signal;
        }
    }

    CRITICAL_SECTION_LEAVE(cs_assert);

    return p_handle_iter;
}
#endif /* CONFIG_TFM_SPM_BACKEND_IPC == 1 */

struct service_t *tfm_spm_get_service_by_sid(uint32_t sid)
{
    struct service_t *p_prev, *p_curr;

    UNI_LIST_FOREACH_NODE_PREV(p_prev, p_curr, &services_listhead, next) {
        if (p_curr->p_ldinf->sid == sid) {
            UNI_LIST_MOVE_AFTER(&services_listhead, p_prev, p_curr, next);
            return p_curr;
        }
    }

    return NULL;
}

#if CONFIG_TFM_DOORBELL_API == 1
/**
 * \brief                   Get the partition context by partition ID.
 *
 * \param[in] partition_id  Partition identity
 *
 * \retval NULL             Failed
 * \retval "Not NULL"       Target partition context pointer,
 *                          \ref partition_t structures
 */
struct partition_t *tfm_spm_get_partition_by_id(int32_t partition_id)
{
    struct partition_t *p_part;

    UNI_LIST_FOREACH(p_part, PARTITION_LIST_ADDR, next) {
        if (p_part->p_ldinf->pid == partition_id) {
            return p_part;
        }
    }

    return NULL;
}
#endif /* CONFIG_TFM_DOORBELL_API == 1 */

int32_t tfm_spm_check_client_version(struct service_t *service,
                                     uint32_t version)
{
    SPM_ASSERT(service);

    switch (SERVICE_GET_VERSION_POLICY(service->p_ldinf->flags)) {
    case SERVICE_VERSION_POLICY_RELAXED:
        if (version > service->p_ldinf->version) {
            return SPM_ERROR_VERSION;
        }
        break;
    case SERVICE_VERSION_POLICY_STRICT:
        if (version != service->p_ldinf->version) {
            return SPM_ERROR_VERSION;
        }
        break;
    default:
        return SPM_ERROR_VERSION;
    }
    return PSA_SUCCESS;
}

int32_t tfm_spm_check_authorization(uint32_t sid,
                                    struct service_t *service,
                                    bool ns_caller)
{
    struct partition_t *partition = NULL;
    const uint32_t *dep;
    int32_t i;

    SPM_ASSERT(service);

    if (ns_caller) {
        if (!SERVICE_IS_NS_ACCESSIBLE(service->p_ldinf->flags)) {
            return SPM_ERROR_GENERIC;
        }
    } else {
        partition = GET_CURRENT_COMPONENT();
        if (!partition) {
            tfm_core_panic();
        }

        dep = LOAD_INFO_DEPS(partition->p_ldinf);
        for (i = 0; i < partition->p_ldinf->ndeps; i++) {
            if (dep[i] == sid) {
                break;
            }
        }

        if (i == partition->p_ldinf->ndeps) {
            return SPM_ERROR_GENERIC;
        }
    }
    return PSA_SUCCESS;
}

/* Message functions */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
struct connection_t *spm_get_client_connection(psa_handle_t handle,
                                               int32_t client_id)
{
    struct connection_t *p_conn_handle = handle_to_connection(handle);

    if (spm_validate_connection(p_conn_handle) != PSA_SUCCESS) {
        return NULL;
    }

    /* Validate the caller id in the connection handle equals client_id. */
    if (p_conn_handle->msg.client_id != client_id) {
        return NULL;
    }

    return p_conn_handle;
}
#endif

struct connection_t *spm_msg_handle_to_connection(psa_handle_t msg_handle)
{
    /*
     * The message handler passed by the caller is considered invalid in the
     * following cases:
     *   1. Not a valid message handle. (The address of a message is not the
     *      address of a possible handle from the pool
     *   2. Handle not belongs to the caller partition (The handle is either
     *      unused, or owned by another partition)
     * Check the conditions above
     */
    int32_t partition_id;
    struct connection_t *p_conn_handle = handle_to_connection(msg_handle);

    if (spm_validate_connection(p_conn_handle) != PSA_SUCCESS) {
        return NULL;
    }

    /* Check that the running partition owns the message */
    partition_id = tfm_spm_partition_get_running_partition_id();
    if (partition_id != p_conn_handle->service->partition->p_ldinf->pid) {
        return NULL;
    }

    return p_conn_handle;
}

void spm_init_connection(struct connection_t *p_connection,
                         struct service_t *service,
                         int32_t client_id)
{
    SPM_ASSERT(p_connection);
    SPM_ASSERT(service);

    /* Clear message buffer before using it */
    spm_memset(&p_connection->msg, 0, sizeof(psa_msg_t));

    p_connection->service = service;
    p_connection->p_client = GET_CURRENT_COMPONENT();
    p_connection->msg.client_id = client_id;
    /* Use the user connect handle as the message handle */
    p_connection->msg.handle = connection_to_handle(p_connection);

    p_connection->status = TFM_HANDLE_STATUS_IDLE;
#if PSA_FRAMEWORK_HAS_MM_IOVEC
    p_connection->iovec_status = 0;
#endif

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX
    /* Set the private data of NSPE client caller in multi-core topology */
    if (IS_NS_AGENT_MAILBOX(p_connection->p_client->p_ldinf)
         && TFM_CLIENT_ID_IS_NS(client_id)) {
        tfm_rpc_set_caller_data(p_connection, client_id);
    } else {
        p_connection->caller_data = NULL;
    }
#endif
}

int32_t tfm_spm_partition_get_running_partition_id(void)
{
    struct partition_t *partition;

    partition = GET_CURRENT_COMPONENT();
    if (partition && partition->p_ldinf) {
        return partition->p_ldinf->pid;
    } else {
        return INVALID_PARTITION_ID;
    }
}

bool tfm_spm_is_ns_caller(void)
{
    struct partition_t *partition = GET_CURRENT_COMPONENT();

    if (!partition) {
        tfm_core_panic();
    }

    return IS_NS_AGENT(partition->p_ldinf);
}

int32_t tfm_spm_get_client_id(bool ns_caller)
{
    int32_t client_id;

    if (ns_caller) {
        client_id = tfm_nspm_get_current_client_id();
    } else {
        client_id = tfm_spm_partition_get_running_partition_id();
    }

    if (ns_caller != (client_id < 0)) {
        /* NS client ID must be negative and Secure ID must >= 0 */
        tfm_core_panic();
    }

    return client_id;
}

uint32_t tfm_spm_init(void)
{
    struct partition_t *partition;
    uint32_t service_setting;
    fih_int fih_rc = FIH_FAILURE;

    spm_init_connection_space();

    UNI_LISI_INIT_NODE(PARTITION_LIST_ADDR, next);
    UNI_LISI_INIT_NODE(&services_listhead, next);

    /* Init the nonsecure context. */
    tfm_nspm_ctx_init();

    while (1) {
        partition = load_a_partition_assuredly(PARTITION_LIST_ADDR);
        if (partition == NO_MORE_PARTITION) {
            break;
        }

        service_setting = load_services_assuredly(
                                partition,
                                &services_listhead,
                                stateless_services_ref_tbl,
                                sizeof(stateless_services_ref_tbl));

        load_irqs_assuredly(partition);

        /* Bind the partition with platform. */
        FIH_CALL(tfm_hal_bind_boundary, fih_rc, partition->p_ldinf,
                 &partition->boundary);
        if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
            tfm_core_panic();
        }

        backend_init_comp_assuredly(partition, service_setting);
    }

    return backend_system_run();
}

void update_caller_outvec_len(struct connection_t *handle)
{
    uint32_t i;

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
        if (handle->msg.out_size[i] == 0) {
            continue;
        }

        SPM_ASSERT(handle->caller_outvec[i].base == handle->outvec_base[i]);

        handle->caller_outvec[i].len = handle->outvec_written[i];
    }
}
