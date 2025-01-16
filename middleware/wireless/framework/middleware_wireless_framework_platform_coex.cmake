# Add set(CONFIG_USE_middleware_wireless_framework_platform_coex true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_DEVICE_ID STREQUAL RW610 OR CONFIG_DEVICE_ID STREQUAL RW612))
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/rw61x/fwk_platform_coex.c
        )
    endif()

  

