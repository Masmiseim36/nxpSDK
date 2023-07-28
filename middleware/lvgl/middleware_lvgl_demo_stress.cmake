# Add set(CONFIG_USE_middleware_lvgl_demo_stress true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lvgl)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/stress/lv_demo_stress.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/stress
)

else()

message(SEND_ERROR "middleware_lvgl_demo_stress dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
