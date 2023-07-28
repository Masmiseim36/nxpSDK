# Add set(CONFIG_USE_middleware_se_hostlib_commonInfra true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_connect.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/global_platf.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_apdu.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_errors.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_printf.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/a71_debug.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra
)

