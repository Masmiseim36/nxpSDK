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

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot
  ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/inc
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DAZ_NO_PRECONDITION_CHECKING
  )

endif()

else()

message(SEND_ERROR "middleware_azure_rtos_azure_iot dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
