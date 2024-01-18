/* 
 * Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _IOT_AGENT_MQTT_FREERTOS_H_
#define _IOT_AGENT_MQTT_FREERTOS_H_

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

iot_agent_status_t iot_agent_verify_mqtt_connection_for_service(iot_agent_context_t* iot_agent_context, const nxp_iot_ServiceDescriptor* service_descriptor);

iot_agent_status_t iot_agent_verify_mqtt_connection(iot_agent_context_t* iot_agent_context);

iot_agent_status_t iot_agent_cleanup_mqtt_config_files();

#if	(AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
iot_agent_status_t iot_agent_verify_mqtt_connection_cos_over_rtp(iot_agent_context_t* iot_agent_context, const nxp_iot_ServiceDescriptor* service_descriptor);

iot_agent_status_t iot_agent_cleanup_mqtt_config_files_cos_over_rtp();
#endif
#ifdef __cplusplus
} // extern "C"
#endif
  /*!
  *@}
  */ /* end of edgelock2go_agent_mqtt */

#endif // #ifndef _IOT_AGENT_MQTT_FREERTOS_H_
