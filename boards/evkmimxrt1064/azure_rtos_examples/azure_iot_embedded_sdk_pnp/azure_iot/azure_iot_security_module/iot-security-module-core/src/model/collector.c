/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#include <stdio.h>
#include <string.h>

#include "asc_security_core/logger.h"
#include "asc_security_core/collector.h"

OBJECT_POOL_DEFINITIONS(collector_t, COLLECTOR_OBJECT_POOL_COUNT)
LINKED_LIST_DEFINITIONS(collector_t)

collector_t *collector_init(collector_init_function_t collector_internal_init_function)
{
    asc_result_t result = ASC_RESULT_OK;
    collector_t *collector_ptr = NULL;

    if (collector_internal_init_function == NULL) {
        log_error("Failed to initialize collector due to bad argument");
        result = ASC_RESULT_BAD_ARGUMENT;
        goto cleanup;
    }

    collector_ptr = object_pool_get(collector_t);
    if (collector_ptr == NULL) {
        log_error("Failed to allocate collector");
        result = ASC_RESULT_MEMORY_EXCEPTION;
        goto cleanup;
    }

    memset(collector_ptr, 0, sizeof(collector_t));

    result = collector_internal_init_function(&collector_ptr->internal);
    if (result != ASC_RESULT_OK) {
        log_error("Failed to initialize collector internal, result=[%d]", result);
        goto cleanup;
    }

    log_debug("Initialize collector, type=[%d], priority=[%d]", collector_ptr->internal.type, collector_ptr->internal.priority);

    // default parameters
    collector_ptr->enabled = true;
    collector_ptr->status = COLLECTOR_STATUS_OK;
    collector_ptr->failure_count = 0;
    collector_ptr->last_collected_timestamp = 0;
    collector_ptr->last_sent_timestamp = 0;

cleanup:
    if (result != ASC_RESULT_OK) {
        log_error("Failed to initialize collector, result=[%d]", result);
        collector_deinit(collector_ptr);
        collector_ptr = NULL;
    }

    return collector_ptr;
}

void collector_deinit(collector_t *collector_ptr)
{
    if (collector_ptr == NULL) {
        return;
    }
    log_debug("Deinitialize collector, type=[%d], priority=[%d]", collector_ptr->internal.type, collector_ptr->internal.priority);

    if (collector_ptr->internal.deinit_function != NULL) {
        collector_ptr->internal.deinit_function(&collector_ptr->internal);
    }

    object_pool_free(collector_t, collector_ptr);
    collector_ptr = NULL;
}

collector_priority_t collector_get_priority(collector_t *collector_ptr)
{
    collector_priority_t priority = COLLECTOR_PRIORITY_HIGH;

    if (collector_ptr == NULL) {
        log_error("Failed to retrieve collector priority, bad argument");
    } else {
        priority = collector_ptr->internal.priority;
    }

    return priority;
}

uint32_t collector_get_last_collected_timestamp(collector_t *collector_ptr)
{
    uint32_t last_collected_timestamp = 0;

    if (collector_ptr == NULL) {
        log_error("Failed to retrieve collector last collected timestamp, bad argument");
    } else {
        last_collected_timestamp = collector_ptr->last_collected_timestamp;
    }

    return last_collected_timestamp;
}

asc_result_t collector_set_last_collected_timestamp(collector_t *collector_ptr, uint32_t last_collected_timestamp)
{
    if (collector_ptr == NULL) {
        log_error("Failed to set collector last collected timestamp, bad argument");
        return ASC_RESULT_BAD_ARGUMENT;
    }

    collector_ptr->last_collected_timestamp = last_collected_timestamp;

    return ASC_RESULT_OK;
}

asc_result_t collector_serialize_events(collector_t *collector_ptr, serializer_t *serializer)
{
    if (collector_ptr == NULL || serializer == NULL) {
        log_error("Collector failed to serialize events, bad argument");
        return ASC_RESULT_BAD_ARGUMENT;
    }

    collector_ptr->last_collected_timestamp = itime_time(NULL);

    if (collector_ptr->internal.collect_function == NULL) {
        log_error("Collector failed to serialize events, internal collect function is NULL");
        return ASC_RESULT_EXCEPTION;
    }

    return collector_ptr->internal.collect_function(&collector_ptr->internal, serializer);
}
