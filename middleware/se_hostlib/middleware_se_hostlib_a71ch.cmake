# Add set(CONFIG_USE_middleware_se_hostlib_a71ch true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_common_A71CH)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/a71ch/src/a71ch_com_scp.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/a71ch/src/a71ch_crypto_derive.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/a71ch/src/a71ch_crypto_ecc.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/a71ch/src/a71ch_module.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/a71ch/src/a71ch_sst.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/api/src/A71HLSEWrapper.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/api/src/ax_crypto_aes_key.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/api/src/ax_crypto_ecc.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/api/src/ax_crypto_rng.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/api/src/ax_scp.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/api/src/ax_switch.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/api/src/ax_util.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/a71_debug.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/log
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/inc
  ${CMAKE_CURRENT_LIST_DIR}/sss/inc
  ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/inc
)

else()

message(SEND_ERROR "middleware_se_hostlib_a71ch dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
