/* 
 * Copyright 2019-2021,2024-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _IOT_AGENT_MQTT_FREERTOS_H_
#define _IOT_AGENT_MQTT_FREERTOS_H_

#include "nxp_iot_agent_common.h"
   
#ifdef __cplusplus
extern "C" {
#endif

 /*! @ingroup edgelock2go_agent_mqtt
 *
 * @page page_macros MQTT PubSub code for FreeRTOS
 *
 * @brief 
 *
 *
 * @addtogroup edgelock2go_agent_mqtt
 * @{
 */
#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
iot_agent_status_t iot_agent_verify_mqtt_connection_for_service(iot_agent_context_t* iot_agent_context, const nxp_iot_ServiceDescriptor* service_descriptor);

iot_agent_status_t iot_agent_verify_mqtt_connection(iot_agent_context_t* iot_agent_context);

iot_agent_status_t iot_agent_cleanup_mqtt_config_files(void);
#endif

#if	(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
iot_agent_status_t iot_agent_verify_mqtt_connection_cos_over_rtp(iot_agent_context_t* iot_agent_context, const nxp_iot_ServiceDescriptor* service_descriptor);

iot_agent_status_t iot_agent_cleanup_mqtt_config_files_cos_over_rtp(void);
#endif
#ifdef __cplusplus
} // extern "C"
#endif
  /*!
  *@}
  */ /* end of edgelock2go_agent_mqtt */

#endif // #ifndef _IOT_AGENT_MQTT_FREERTOS_H_
