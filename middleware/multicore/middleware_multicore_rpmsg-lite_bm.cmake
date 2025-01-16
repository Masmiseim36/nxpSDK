# Add set(CONFIG_USE_middleware_multicore_rpmsg-lite_bm true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/rpmsg-lite/lib/rpmsg_lite/porting/environment/rpmsg_env_bm.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/rpmsg-lite/lib/include/environment/bm
        )

  
