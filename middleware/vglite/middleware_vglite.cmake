# Add set(CONFIG_USE_middleware_vglite true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite.c
  ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_image.c
  ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_matrix.c
  ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_path.c
  ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_flat.c
  ${CMAKE_CURRENT_LIST_DIR}/VGLite/rtos/vg_lite_os.c
  ${CMAKE_CURRENT_LIST_DIR}/font/buf_reader.c
  ${CMAKE_CURRENT_LIST_DIR}/font/rle_font_read.c
  ${CMAKE_CURRENT_LIST_DIR}/font/vft_debug.c
  ${CMAKE_CURRENT_LIST_DIR}/font/vft_draw.c
  ${CMAKE_CURRENT_LIST_DIR}/font/vg_lite_text.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_bwfont.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_encoding.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_font.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_justify.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_kerning.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_rlefont.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_scaledfont.c
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder/mf_wordwrap.c
  ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel/vg_lite_kernel.c
  ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel/rtos/vg_lite_hal.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/inc
  ${CMAKE_CURRENT_LIST_DIR}/font
  ${CMAKE_CURRENT_LIST_DIR}/font/mcufont/decoder
  ${CMAKE_CURRENT_LIST_DIR}/VGLite/rtos
  ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel
  ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel/rtos
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DVG_COMMAND_CALL=1
    -DVG_TARGET_FAST_CLEAR=0
  )

endif()

