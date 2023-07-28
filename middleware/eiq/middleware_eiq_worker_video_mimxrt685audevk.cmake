# Add set(CONFIG_USE_middleware_eiq_worker_video_mimxrt685audevk true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_worker)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_camera.c
  ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_display.c
  ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_pxp.c
  ${CMAKE_CURRENT_LIST_DIR}/common/video/eiq_video_worker.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/common/video
)

else()

message(SEND_ERROR "middleware_eiq_worker_video_mimxrt685audevk dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
