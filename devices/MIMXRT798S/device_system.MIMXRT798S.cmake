# Add set(CONFIG_USE_device_system true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_CORE_ID STREQUAL cm33_core0)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT798S_cm33_core0.c
        )
    endif()

        if(CONFIG_CORE_ID STREQUAL cm33_core1)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT798S_cm33_core1.c
        )
    endif()

        if(CONFIG_CORE_ID STREQUAL ezhv)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT798S_ezhv.c
        )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE_ID STREQUAL hifi1)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT798S_hifi1.c
        )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE_ID STREQUAL hifi4)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT798S_hifi4.c
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
