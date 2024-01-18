# Add set(CONFIG_USE_middleware_eiq_mpp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_camera_mipi_ov5640.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_camera_ezh_ov7670.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_display_lcdifv2_rk055ahd091.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_camera_csi_mt9m114.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_display_lcdif_rk043fn.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_display_mculcd_ssd1963.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_draw.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_freertos.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_graphics_pxp.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_graphics_cpu.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_static_image.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_vision_algo_tflite.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_vision_algo_glow.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/hal_vision_algo_deep_view_rt.c
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/tflite/model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/tflite/model_all_ops_micro.cpp
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/glow/model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/deep_view_rt/model.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/mpp/include
  ${CMAKE_CURRENT_LIST_DIR}/mpp/hal/include
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --compiler_language=auto
      --diag_suppress Pa082,Pa084,Pa093,Pe111,Pe161,Pe174,Pe177,Pe186,Pe550,Pe611,Pe997,Pe1444
      --dlib_config full
      --enable_restrict
      --fno-rtti
      --fno-exceptions
      --c++
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL mdk)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -ffp-mode=full
      -fno-exceptions
      -std=gnu++11
      -Wno-c++17-extensions
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL armgcc)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wall
      -Wno-strict-aliasing
      -fno-rtti
      -fno-exceptions
      -Wno-sign-compare
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL mcux)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wno-strict-aliasing
      -fno-rtti
      -fno-exceptions
      -Wno-sign-compare
    )
  endif()

endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/mpp/lib/cm7/libmpp.a
      -Wl,--end-group
  )
endif()

