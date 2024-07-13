# Add set(CONFIG_USE_middleware_se_hostlib_nxp_iot_agent_common true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/src/utils/iot_agent_mqtt_freertos.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/platform/mbedtls/network_mbedtls.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/platform/mbedtls/net_lwip.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_common.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_config.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_datastore.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_datastore_fs.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_datastore_plain.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_keystore.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_keystore_psa.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_keystore_sss_se05x.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_service.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_session.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_time.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/nxp_iot_agent_utils_protobuf_socket.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/common/nxp_iot_agent_dispatcher.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/common/nxp_iot_agent_utils_protobuf.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/Agent.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/Apdu.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/Datastore.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/Dispatcher.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/Hostcmd.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/PSA.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/RPC.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/ServiceDescriptor.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/Types.pb.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/pb_common.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/pb_decode.c
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf/pb_encode.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/inc
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/ex/inc
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/platform
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/platform/mbedtls
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/src/protobuf
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DPB_FIELD_32BIT
    -DEXTERNAL_CUSTOMER_BUILD_CONFIGURATION=1
  )

endif()

