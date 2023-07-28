# Add set(CONFIG_USE_middleware_se_hostlib_commonSe050CloudDemos true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_commonSe050 AND CONFIG_USE_middleware_se_hostlib_commonCloudDemos AND CONFIG_USE_middleware_se_hostlib_mbedtls_sss)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/sss/inc
  ${CMAKE_CURRENT_LIST_DIR}/sss/ex/inc
  ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x_03_xx_xx
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/log
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/inc
  ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/https_client
)

else()

message(SEND_ERROR "middleware_se_hostlib_commonSe050CloudDemos dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
