# Add set(CONFIG_USE_middleware_azure_rtos_azure_iot true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_azure_rtos_nxd OR CONFIG_USE_middleware_azure_rtos_nxd_sp)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot_adu_agent.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot_adu_root_key.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot_hub_client.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot_hub_client_properties.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot_json_reader.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot_json_writer.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/nx_azure_iot_provisioning_client.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_base64.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_pipeline.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_logging.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_retry.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_request.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_response.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_common.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_c2d.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_commands.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_methods.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_properties.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_sas.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_telemetry.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_twin.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client_sas.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/platform/az_nohttp.c
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/platform/az_noplatform.c
)

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mdk))
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/nx_azure_iot_security_module.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime/builder.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime/emitter.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime/refmap.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/collector_collection.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/collectors/collector_heartbeat.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/collectors_info.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/components_factory.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/components_manager.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/core.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/logger.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/model/collector.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/model/security_message.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/object_pool_static.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/extensions/custom_builder_allocator.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/extensions/page_allocator.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/heartbeat.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/network_activity.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/serializer.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/serializer_private.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/system_information.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/collection/bit_vector.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/collection/hashtable.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/collection/list.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/collection/stack.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/event_loop_be.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/iconv.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/notifier.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/string_utils.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/uuid.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/src/collectors/collector_network_activity.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/src/collectors/collector_system_information.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/src/model/objects/object_network_activity_ext.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/ievent_loop.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/irand.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/itime.c
      ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/os_utils.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/inc
)

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mdk))
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/inc/configs/RTOS_BASE
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/inc
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/include
)
endif()

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DAZ_NO_PRECONDITION_CHECKING
  )

endif()

else()

message(SEND_ERROR "middleware_azure_rtos_azure_iot dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
