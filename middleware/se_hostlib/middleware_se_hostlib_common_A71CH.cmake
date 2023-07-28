# Add set(CONFIG_USE_middleware_se_hostlib_common_A71CH true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/scp/scp_a7x.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/ksdk/ax_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/ksdk/i2c_frdm.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/ksdk/i2c_imxrt.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis_bm.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/ksdk/timer_kinetis_freertos.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/inc
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/inc
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/platform/ksdk
  ${CMAKE_CURRENT_LIST_DIR}/sss/inc
  ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/scp
  ${CMAKE_CURRENT_LIST_DIR}/platform
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTGT_A71CH
    -DSCI2C
    -DSSS_USE_FTR_FILE
  )

endif()

