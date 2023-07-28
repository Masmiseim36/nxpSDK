# Add set(CONFIG_USE_middleware_tfm_s_mimxrt685audevk true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL mimxrt685audevk))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/target_cfg.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/Device/Include
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/partition
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTARGET_CONFIG_HEADER_FILE="config_tfm_target.h"
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_s_mimxrt685audevk dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
