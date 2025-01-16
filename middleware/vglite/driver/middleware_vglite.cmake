# Add set(CONFIG_USE_middleware_vglite true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite.c
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_image.c
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_matrix.c
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_path.c
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/vg_lite_stroke.c
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/rtos/vg_lite_os.c
          ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel/vg_lite_kernel.c
          ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel/rtos/vg_lite_hal.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/inc
          ${CMAKE_CURRENT_LIST_DIR}/VGLite
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/rtos
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/Series/gc555/0x423_ECO
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/Series/gc355/0x0_1216
          ${CMAKE_CURRENT_LIST_DIR}/VGLite/Series/gc255/0x40A
          ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel
          ${CMAKE_CURRENT_LIST_DIR}/VGLiteKernel/rtos
        )

  
