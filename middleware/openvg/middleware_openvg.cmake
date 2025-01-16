# Add set(CONFIG_USE_middleware_openvg true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/vg_api.c
          ${CMAKE_CURRENT_LIST_DIR}/src/vg_context.c
          ${CMAKE_CURRENT_LIST_DIR}/src/vg_egl.c
          ${CMAKE_CURRENT_LIST_DIR}/src/vg_image.c
          ${CMAKE_CURRENT_LIST_DIR}/src/vg_vgu.c
          ${CMAKE_CURRENT_LIST_DIR}/src/vg_egl_freertos.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/include/Vivante
          ${CMAKE_CURRENT_LIST_DIR}/include/EGL
          ${CMAKE_CURRENT_LIST_DIR}/include/VG
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  
            if(CONFIG_TOOLCHAIN STREQUAL armgcc)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              -Wno-enum-compare
            )
      endif()
      
  endif()

