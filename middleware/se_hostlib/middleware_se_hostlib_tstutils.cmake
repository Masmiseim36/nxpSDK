# Add set(CONFIG_USE_middleware_se_hostlib_tstutils true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_common_A71CH)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_app_boot.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil/hkdf_mbedtls.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil/tst_a71ch_util.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil/tst_sm_time_kinetis.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil/tst_sm_util.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils_rtos.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil/tstHostCrypto_mbedtls.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil/tstHostCrypto_mbedtls_aes.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil
)

else()

message(SEND_ERROR "middleware_se_hostlib_tstutils dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
