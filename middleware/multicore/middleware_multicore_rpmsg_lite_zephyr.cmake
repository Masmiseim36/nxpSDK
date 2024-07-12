# Add set(CONFIG_USE_middleware_multicore_rpmsg_lite_zephyr true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_multicore_rpmsg_lite)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_zephyr.c
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/rpmsg_queue.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include/environment/zephyr
)

else()

message(SEND_ERROR "middleware_multicore_rpmsg_lite_zephyr dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
