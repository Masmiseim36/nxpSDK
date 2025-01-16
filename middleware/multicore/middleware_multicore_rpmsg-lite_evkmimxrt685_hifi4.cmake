# Add set(CONFIG_USE_middleware_multicore_rpmsg-lite_evkmimxrt685_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/rpmsg-lite/lib/rpmsg_lite/porting/platform/imxrt600_hifi4/rpmsg_platform.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/rpmsg-lite/lib/include/platform/imxrt600_hifi4
        )

  
