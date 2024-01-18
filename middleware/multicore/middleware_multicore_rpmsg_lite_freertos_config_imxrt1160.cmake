# Add set(CONFIG_USE_middleware_multicore_rpmsg_lite_freertos_config_imxrt1160 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_multicore_rpmsg_lite AND CONFIG_USE_middleware_multicore_rpmsg_lite_freertos AND CONFIG_USE_middleware_multicore_mcmgr_imxrt1160 AND (CONFIG_BOARD STREQUAL evkmimxrt1160) AND CONFIG_USE_middleware_multicore_rpmsg_lite_imxrt1160_freertos)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/template_application/rpmsg_config.h "" middleware_multicore_rpmsg_lite_freertos_config_imxrt1160)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/template_application
)

else()

message(SEND_ERROR "middleware_multicore_rpmsg_lite_freertos_config_imxrt1160 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
