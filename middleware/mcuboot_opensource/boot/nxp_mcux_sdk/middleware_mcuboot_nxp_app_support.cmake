# Add set(CONFIG_USE_middleware_mcuboot_nxp_app_support true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/app_support/mcuboot_app_support.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/app_support
          ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/include/mcuboot_config
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
