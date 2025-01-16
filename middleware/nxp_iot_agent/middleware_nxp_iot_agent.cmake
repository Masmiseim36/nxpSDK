# Add set(CONFIG_USE_middleware_nxp_iot_agent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/ex/src/osal/freertos/iot_agent_osal_freertos.c
          ${CMAKE_CURRENT_LIST_DIR}/ex/src/utils/iot_agent_mqtt_freertos.c
          ${CMAKE_CURRENT_LIST_DIR}/net_crypto/mbedtls/network_mbedtls.c
          ${CMAKE_CURRENT_LIST_DIR}/net_crypto/mbedtls/net_lwip.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent_common.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent_config.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent_datastore.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent_keystore.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent_service.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent_utils.c
          ${CMAKE_CURRENT_LIST_DIR}/src/nxp_iot_agent_utils_protobuf_socket.c
          ${CMAKE_CURRENT_LIST_DIR}/src/common/nxp_iot_agent_dispatcher.c
          ${CMAKE_CURRENT_LIST_DIR}/src/common/nxp_iot_agent_utils_protobuf.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Agent.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Apdu.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Datastore.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Dispatcher.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Hostcmd.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/PSA.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Proxy.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/RPC.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/ServiceDescriptor.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/Types.pb.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/pb_common.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/pb_decode.c
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf/pb_encode.c
          ${CMAKE_CURRENT_LIST_DIR}/src/datastore/plain/nxp_iot_agent_datastore_plain.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/inc
          ${CMAKE_CURRENT_LIST_DIR}/src
          ${CMAKE_CURRENT_LIST_DIR}/ex/inc
          ${CMAKE_CURRENT_LIST_DIR}/net_crypto
          ${CMAKE_CURRENT_LIST_DIR}/net_crypto/mbedtls
          ${CMAKE_CURRENT_LIST_DIR}/src/protobuf
          ${CMAKE_CURRENT_LIST_DIR}/ex/src/osal/freertos
          ${CMAKE_CURRENT_LIST_DIR}/src/datastore/plain
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DPB_FIELD_32BIT
                        -DNXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS=1
              )
  
  
  endif()

