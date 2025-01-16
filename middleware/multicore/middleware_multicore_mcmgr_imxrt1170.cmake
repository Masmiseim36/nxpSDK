# Add set(CONFIG_USE_middleware_multicore_mcmgr_imxrt1170 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src/mcmgr_internal_core_api_imxrt1170.c
          ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src/mcmgr_mu_internal.c
        )

  

