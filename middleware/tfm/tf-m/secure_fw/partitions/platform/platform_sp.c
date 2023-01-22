/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_sp.h"

#include "tfm_platform_system.h"
#include "tfm_plat_nv_counters.h"
#include "load/partition_defs.h"
#include "psa_manifest/pid.h"

#define NV_COUNTER_ID_SIZE  sizeof(enum tfm_nv_counter_t)

#ifdef TFM_PSA_API
#include "psa/client.h"
#include "psa/service.h"
#include "region_defs.h"
#include "psa_manifest/tfm_platform.h"

#ifndef INPUT_BUFFER_SIZE
#define INPUT_BUFFER_SIZE      64
#endif

#ifndef OUTPUT_BUFFER_SIZE
#define OUTPUT_BUFFER_SIZE     64
#endif

typedef enum tfm_platform_err_t (*plat_func_t)(const psa_msg_t *msg);
#else
#include "tfm_secure_api.h"
#endif /* TFM_PSA_API */

enum tfm_platform_err_t platform_sp_system_reset(void)
{
    /* FIXME: The system reset functionality is only supported in isolation
     *        level 1.
     */

    tfm_platform_hal_system_reset();

    return TFM_PLATFORM_ERR_SUCCESS;
}

static enum tfm_platform_err_t nv_counter_permissions_check(
        int32_t client_id,
        enum tfm_nv_counter_t nv_counter_no,
        bool is_read)
{
    /* Not used currently */
    (void)is_read;

    switch (nv_counter_no) {
#ifdef TFM_PARTITION_PROTECTED_STORAGE
    case PLAT_NV_COUNTER_PS_0:
    case PLAT_NV_COUNTER_PS_1:
    case PLAT_NV_COUNTER_PS_2:
        if (client_id == TFM_SP_PS) {
            return TFM_PLATFORM_ERR_SUCCESS;
        } else {
            return TFM_PLATFORM_ERR_NOT_SUPPORTED;
        }
#endif
    case PLAT_NV_COUNTER_NS_0:
    case PLAT_NV_COUNTER_NS_1:
    case PLAT_NV_COUNTER_NS_2:
        /* TODO how does this interact with the ns_ctx extension? */
        if (client_id < 0) {
            return TFM_PLATFORM_ERR_SUCCESS;
        } else {
            return TFM_PLATFORM_ERR_NOT_SUPPORTED;
        }
    default:
        return TFM_PLATFORM_ERR_NOT_SUPPORTED;
    }
}

#ifndef TFM_PSA_API

enum tfm_platform_err_t
platform_sp_ioctl(psa_invec  *in_vec,  uint32_t num_invec,
                  psa_outvec *out_vec, uint32_t num_outvec)
{
    void *input, *output;
    tfm_platform_ioctl_req_t request;

    if ((num_invec < 1) || (num_invec > 2) ||
        (num_outvec > 1) ||
        (in_vec[0].base == NULL) ||
        (in_vec[0].len != sizeof(tfm_platform_ioctl_req_t))) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    input = (num_invec == 1) ? NULL : &in_vec[1];
    output = out_vec;
    request = *((tfm_platform_ioctl_req_t *)in_vec[0].base);

    return tfm_platform_hal_ioctl(request, input, output);
}

enum tfm_platform_err_t
platform_sp_nv_counter_read(psa_invec  *in_vec,  uint32_t num_invec,
                            psa_outvec *out_vec, uint32_t num_outvec)
{
    enum tfm_plat_err_t err;
    enum tfm_nv_counter_t counter_id;
    uint32_t counter_size;
    int32_t status, client_id;

    if (in_vec[0].len != NV_COUNTER_ID_SIZE ||
        num_invec != 1 || num_outvec != 1) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }
    counter_id = *((enum tfm_nv_counter_t *)in_vec[0].base);
    counter_size = out_vec[0].len;

    status = tfm_core_get_caller_client_id(&client_id);
    if (status != (int32_t)TFM_SUCCESS) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }
    if (client_id < 0) {
        counter_id += PLAT_NV_COUNTER_NS_0;
    }

    if (nv_counter_permissions_check(client_id, counter_id, true)
        != TFM_PLAT_ERR_SUCCESS) {
       return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }
    err = tfm_plat_read_nv_counter(counter_id, counter_size,
                                   (uint8_t *)out_vec[0].base);
    if (err != TFM_PLAT_ERR_SUCCESS) {
       return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    return TFM_PLATFORM_ERR_SUCCESS;
}

enum tfm_platform_err_t
platform_sp_nv_counter_increment(psa_invec  *in_vec,  uint32_t num_invec,
                                 psa_outvec *out_vec, uint32_t num_outvec)
{
    enum tfm_plat_err_t err;
    enum tfm_nv_counter_t counter_id;
    int32_t client_id, status;

    if (in_vec[0].len != NV_COUNTER_ID_SIZE ||
        num_invec != 1 || num_outvec != 0) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    status = tfm_core_get_caller_client_id(&client_id);
    if (status != (int32_t)TFM_SUCCESS) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    counter_id = *((enum tfm_nv_counter_t *)in_vec[0].base);
    if (client_id < 0) {
        counter_id += PLAT_NV_COUNTER_NS_0;
    }

    if (nv_counter_permissions_check(client_id, counter_id, false)
        != TFM_PLAT_ERR_SUCCESS) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }
    err = tfm_plat_increment_nv_counter(counter_id);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    return TFM_PLATFORM_ERR_SUCCESS;
}

#else /* TFM_PSA_API */

static psa_status_t platform_sp_system_reset_psa_api(const psa_msg_t *msg)
{
    (void)msg; /* unused parameter */

    return platform_sp_system_reset();
}

static psa_status_t platform_sp_nv_read_psa_api(const psa_msg_t *msg)
{
    enum tfm_plat_err_t err = TFM_PLAT_ERR_SYSTEM_ERR;
    size_t in_len = PSA_MAX_IOVEC, out_len = PSA_MAX_IOVEC, num = 0;

    enum tfm_nv_counter_t counter_id;
    //NXP uint8_t counter_val = 0;
    uint8_t counter_val[4] = {0}; //NXP must be 4 bytes to avoid memory corruption

    /* Check the number of in_vec filled */
    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0)) {
        in_len--;
    }

    /* Check the number of out_vec filled */
    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0)) {
        out_len--;
    }

    if (msg->in_size[0] != NV_COUNTER_ID_SIZE ||
        in_len != 1 || out_len != 1) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    num = psa_read(msg->handle, 0, &counter_id, msg->in_size[0]);
    if (num != NV_COUNTER_ID_SIZE) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    if (msg->client_id < 0) {
        counter_id += PLAT_NV_COUNTER_NS_0;
    }

    if (nv_counter_permissions_check(msg->client_id, counter_id, true)
        != TFM_PLATFORM_ERR_SUCCESS) {
       return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    err = tfm_plat_read_nv_counter(counter_id,  msg->out_size[0],
                                   counter_val); //NXP

    if (err != TFM_PLAT_ERR_SUCCESS) {
       return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    psa_write(msg->handle, 0, counter_val, msg->out_size[0]); //NXP

    return TFM_PLATFORM_ERR_SUCCESS;
}

static psa_status_t platform_sp_nv_increment_psa_api(const psa_msg_t *msg)
{
    enum tfm_plat_err_t err = TFM_PLAT_ERR_SYSTEM_ERR;
    size_t in_len = PSA_MAX_IOVEC, out_len = PSA_MAX_IOVEC, num = 0;

    enum tfm_nv_counter_t counter_id;

    /* Check the number of in_vec filled */
    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0)) {
        in_len--;
    }

    /* Check the number of out_vec filled */
    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0)) {
        out_len--;
    }

    if (msg->in_size[0] != NV_COUNTER_ID_SIZE ||
        in_len != 1 || out_len != 0) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    num = psa_read(msg->handle, 0, &counter_id, msg->in_size[0]);
    if (num != NV_COUNTER_ID_SIZE) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    if (msg->client_id < 0) {
        counter_id += PLAT_NV_COUNTER_NS_0;
    }

    if (nv_counter_permissions_check(msg->client_id, counter_id, false)
        != TFM_PLATFORM_ERR_SUCCESS) {
       return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    err = tfm_plat_increment_nv_counter(counter_id);

    if (err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    return TFM_PLATFORM_ERR_SUCCESS;
}

static psa_status_t platform_sp_ioctl_psa_api(const psa_msg_t *msg)
{
    void *input = NULL;
    void *output = NULL;
    psa_invec invec = {0};
    psa_outvec outvec = {0};
    uint8_t input_buffer[INPUT_BUFFER_SIZE] = {0};
    uint8_t output_buffer[OUTPUT_BUFFER_SIZE] = {0};
    tfm_platform_ioctl_req_t request = 0;
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SYSTEM_ERROR;
    int num = 0;
    uint32_t in_len = PSA_MAX_IOVEC;
    uint32_t out_len = PSA_MAX_IOVEC;
    size_t input_size;

    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0)) {
        in_len--;
    }

    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0)) {
        out_len--;
    }

    if ((in_len < 1) || (in_len > 2) ||
        (out_len > 1)) {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    num = psa_read(msg->handle, 0, &request, sizeof(request));
    if (num != sizeof(request)) {
        return (enum tfm_platform_err_t) PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_len > 1) {
        input_size = msg->in_size[1];
        if (input_size > INPUT_BUFFER_SIZE) {
            return (enum tfm_platform_err_t) PSA_ERROR_BUFFER_TOO_SMALL;
        }
        num = psa_read(msg->handle, 1, &input_buffer, msg->in_size[1]);
        if (num != input_size) {
            return (enum tfm_platform_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        invec.base = input_buffer;
        invec.len = input_size;
        input = &invec;
    }

    if (out_len > 0) {
        if (msg->out_size[0] > OUTPUT_BUFFER_SIZE) {
            return (enum tfm_platform_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        outvec.base = output_buffer;
        outvec.len = msg->out_size[0];
        output = &outvec;
    }

    ret = tfm_platform_hal_ioctl(request, input, output);

    if (output != NULL) {
        psa_write(msg->handle, 0, outvec.base, outvec.len);
    }

    return ret;
}

psa_status_t tfm_platform_service_sfn(const psa_msg_t *msg)
{
    switch (msg->type) {
    case TFM_PLATFORM_API_ID_NV_READ:
        return platform_sp_nv_read_psa_api(msg);
    case TFM_PLATFORM_API_ID_NV_INCREMENT:
        return platform_sp_nv_increment_psa_api(msg);
    case TFM_PLATFORM_API_ID_SYSTEM_RESET:
        return platform_sp_system_reset_psa_api(msg);
    case TFM_PLATFORM_API_ID_IOCTL:
        return platform_sp_ioctl_psa_api(msg);
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_GENERIC_ERROR;
}
#endif /* TFM_PSA_API */

psa_status_t platform_sp_init(void)
{
    /* Initialise the non-volatile counters */
    enum tfm_plat_err_t err;

    err = tfm_plat_init_nv_counter();
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return PSA_ERROR_HARDWARE_FAILURE;
    }

    return PSA_SUCCESS;
}
