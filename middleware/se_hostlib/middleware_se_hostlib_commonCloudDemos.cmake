# Add set(CONFIG_USE_middleware_se_hostlib_commonCloudDemos true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel_heap_4 AND CONFIG_USE_middleware_freertos_coremqtt AND CONFIG_USE_middleware_pkcs11 AND CONFIG_USE_middleware_freertos_backoffalgorithm)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils_rtos.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_core.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_object.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_asymm.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_symm.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/base64_decode.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/inc
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSSS_USE_FTR_FILE
    -DMBEDTLS
    -DSDK_OS_FREE_RTOS
    -DUSE_RTOS
    -DmqttconfigENABLE_METRICS=0
    -DSE_CLOUD_MCU_SDK
  )

endif()

else()

message(SEND_ERROR "middleware_se_hostlib_commonCloudDemos dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
