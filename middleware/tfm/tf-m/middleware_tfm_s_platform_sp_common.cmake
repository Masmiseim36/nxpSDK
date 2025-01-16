# Add set(CONFIG_USE_middleware_tfm_s_platform_sp_common true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/platform_sp.c
        )

  

