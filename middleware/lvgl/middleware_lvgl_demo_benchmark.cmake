# Add set(CONFIG_USE_middleware_lvgl_demo_benchmark true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lvgl)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/lv_demo_benchmark.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_argb.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/benchmark
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/examples/assets
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/demos/stress/assets
)

else()

message(SEND_ERROR "middleware_lvgl_demo_benchmark dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
