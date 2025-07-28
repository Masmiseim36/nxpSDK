# Add set(CONFIG_USE_middleware_eiq_executorch true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/..
          ${CMAKE_CURRENT_LIST_DIR}/../..
          ${CMAKE_CURRENT_LIST_DIR}/third-party/flatbuffers/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DNO_HEAP_USAGE=1
              )
  
            if(CONFIG_TOOLCHAIN STREQUAL mdk)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              -std=gnu++17
            )
      endif()
          if(CONFIG_TOOLCHAIN STREQUAL armgcc)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              -std=c99
              -std=c++17
            )
      endif()
      
  endif()

