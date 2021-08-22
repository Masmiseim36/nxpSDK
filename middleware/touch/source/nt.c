/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_frmstr_prv.h"
#include "nt.h"

int32_t nt_init(const struct nt_system *system, uint8_t *pool, const uint32_t size)
{
    NT_ASSERT(system != NULL);

    int32_t result = (int32_t)NT_SUCCESS;

    if (_nt_mem_init(pool, size) < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }
    if (_nt_freemaster_init() < (int32_t)NT_SUCCESS)
    {
        return (int32_t)NT_FAILURE;
    }
    result = _nt_system_init(system);
    if ((bool)(result < (int32_t)NT_SUCCESS))
    {
        return result;
    }
    result = _nt_system_module_function((int32_t)NT_SYSTEM_MODULE_INIT);
    if ((bool)(result < (int32_t)NT_SUCCESS))
    {
        return result;
    }
    result = _nt_system_control_function((int32_t)NT_SYSTEM_CONTROL_INIT);
    if ((bool)(result < (int32_t)NT_SUCCESS))
    {
        return result;
    }
    return result;
}

int32_t nt_trigger(void)
{
    int32_t result = (int32_t)NT_SUCCESS;

    _nt_system_increment_time_counter();
    if (_nt_system_module_function((int32_t)NT_SYSTEM_MODULE_TRIGGER) < (int32_t)NT_SUCCESS)
    {
        result = (int32_t)NT_FAILURE;
    }
    if (_nt_system_control_function((int32_t)NT_SYSTEM_CONTROL_OVERRUN) < (int32_t)NT_SUCCESS)
    {
        result = (int32_t)NT_FAILURE;
    }
    if (result == (int32_t)NT_FAILURE)
    {
        /* triggering is faster than measurement/processing */
        _nt_system_invoke_callback((int32_t)NT_SYSTEM_EVENT_OVERRUN, NULL);
    }

    return result;
}

int32_t nt_task(void)
{
    int32_t ret;

    ret = (int32_t)_nt_system_module_function((uint32_t)NT_SYSTEM_MODULE_PROCESS);
    if (ret < (int32_t)NT_SUCCESS)
    {
        return ret;
    }
#if (NT_SAFETY_SUPPORT == 1)
    ret = _nt_system_module_function((uint32_t)NT_SYSTEM_MODULE_SAFETY_PROCESS);
    if (ret < (int32_t)NT_SUCCESS)
    {
        return ret;
    }
#endif /* NT_SAFETY_SUPPORT */

    ret = _nt_system_control_function((uint32_t)NT_SYSTEM_CONTROL_PROCESS);
    if (ret < (int32_t)NT_SUCCESS)
    {
        return ret;
    }
    return (int32_t)NT_SUCCESS;
}

void nt_error(char *file, uint32_t line)
{
/* User's error handling */
#if (NT_DEBUG != 0)
    if ((bool)_nt_system_get()->error_callback)
    {
        _nt_system_get()->error_callback(file, line);
    }
#endif
    while ((bool)1)
    {
    }
}
