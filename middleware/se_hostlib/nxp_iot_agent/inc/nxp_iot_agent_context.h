/* 
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _NXP_IOT_AGENT_CONTEXT_H_
#define _NXP_IOT_AGENT_CONTEXT_H_

#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_service.h>
#include <nxp_iot_agent_keystore.h>
#include <nxp_iot_agent_datastore.h>

/*!
 * @ingroup edgelock2go_agent_main
 *
 * @addtogroup edgelock2go_agent_main
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Container for EdgeLock 2GO agent context
 * @param  keystores Container for keystores
 * @param  datastores Containers for datastores
 * @param  numKeystores Number of keystores currently registered
 * @param  numDatastores Number of datastores currently registered
 * @param  selectedService Current selected service
 * @param  edgelock2go_datastore The datastore which holds information for the connection to the EdgeLock 2GO cloud service.
 */

typedef struct iot_agent_context_t {
    iot_agent_keystore_t* keystores[NXP_IOT_AGENT_MAX_NUM_KEYSTORES];
    iot_agent_datastore_t* datastores[NXP_IOT_AGENT_MAX_NUM_DATASTORES];
    size_t numKeystores;
    size_t numDatastores;
    service_link_t selectedService;

	iot_agent_datastore_t* edgelock2go_datastore;
} iot_agent_context_t;


#ifdef __cplusplus
} // extern "C"
#endif

/*!
 *@}
 */ /* end of edgelock2go_agent_main */

#endif // #ifndef _NXP_IOT_AGENT_CONTEXT_H_
