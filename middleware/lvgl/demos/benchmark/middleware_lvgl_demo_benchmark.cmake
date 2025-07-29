# Add set(CONFIG_USE_middleware_lvgl_demo_benchmark true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/lv_demo_benchmark.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/img_benchmark_avatar.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/img_benchmark_lvgl_logo_argb.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/img_benchmark_lvgl_logo_rgb.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/lv_font_benchmark_montserrat_12_aligned.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/lv_font_benchmark_montserrat_14_aligned.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/lv_font_benchmark_montserrat_16_aligned.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/lv_font_benchmark_montserrat_18_aligned.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/lv_font_benchmark_montserrat_20_aligned.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/lv_font_benchmark_montserrat_24_aligned.c
          ${CMAKE_CURRENT_LIST_DIR}/assets/lv_font_benchmark_montserrat_26_aligned.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/assets
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DLV_CONF_INCLUDE_SIMPLE
              )
  
  
  endif()

