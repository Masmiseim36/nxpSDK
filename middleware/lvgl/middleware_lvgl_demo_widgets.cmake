# Add set(CONFIG_USE_middleware_lvgl_demo_widgets true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lvgl)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/widgets/lv_demo_widgets.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/widgets/assets/img_clothes.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/widgets/assets/img_demo_widgets_avatar.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/widgets/assets/img_lvgl_logo.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/widgets
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/widgets/assets
)

else()

message(SEND_ERROR "middleware_lvgl_demo_widgets dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
