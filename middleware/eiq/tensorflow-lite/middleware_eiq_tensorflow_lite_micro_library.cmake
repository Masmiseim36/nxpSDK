# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_library true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/third_party/kissfft/tools/kfc.c
          ${CMAKE_CURRENT_LIST_DIR}/third_party/kissfft/tools/kiss_fastfir.c
          ${CMAKE_CURRENT_LIST_DIR}/third_party/kissfft/tools/kiss_fftnd.c
          ${CMAKE_CURRENT_LIST_DIR}/third_party/kissfft/tools/kiss_fftndr.c
          ${CMAKE_CURRENT_LIST_DIR}/third_party/kissfft/tools/kiss_fftr.c
          ${CMAKE_CURRENT_LIST_DIR}/third_party/kissfft/kiss_fft.c
          ${CMAKE_CURRENT_LIST_DIR}/third_party/fft2d/fftsg.c
        )

  
  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  
            if(CONFIG_TOOLCHAIN STREQUAL iar)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              --dlib_config
              full
            )
      endif()
      
  endif()

