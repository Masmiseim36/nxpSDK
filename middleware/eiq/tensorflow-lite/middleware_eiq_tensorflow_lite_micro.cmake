# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/../common/toolchains/iar/retarget.c
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/../common/toolchains/mdk/retarget.c
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/third_party/flatbuffers/include
          ${CMAKE_CURRENT_LIST_DIR}/third_party/gemmlowp
          ${CMAKE_CURRENT_LIST_DIR}/third_party/kissfft
          ${CMAKE_CURRENT_LIST_DIR}/third_party/ruy
          ${CMAKE_CURRENT_LIST_DIR}/third_party/cmsis_nn
          ${CMAKE_CURRENT_LIST_DIR}/third_party/cmsis_nn/Include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DTF_LITE_STATIC_MEMORY
              )
  
            if(CONFIG_TOOLCHAIN STREQUAL iar)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              --diag_suppress=Pa082,Pa050,Pe188,Pe069,Go003,Pa050,Pa082,Pa084,Pa093,Pe069,Pe111,Pe161,Pe174,Pe177,Pe186,Pe188,Pe550,Pe611,Pe997,Pe1444,Pe1675,Pe830
            )
      endif()
      
  endif()

