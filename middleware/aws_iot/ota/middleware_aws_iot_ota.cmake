# Add set(CONFIG_USE_middleware_aws_iot_ota true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/ota.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ota_base64.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ota_cbor.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ota_http.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ota_interface.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ota_mqtt.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/source/include
        )

  
