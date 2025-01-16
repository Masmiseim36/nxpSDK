# Add set(CONFIG_USE_middleware_mcuboot_nxp_bootutil_port true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/boot.c
          ${CMAKE_CURRENT_LIST_DIR}/bootutil_hooks.c
          ${CMAKE_CURRENT_LIST_DIR}/keys.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/flashapi
          ${CMAKE_CURRENT_LIST_DIR}/keys
          ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/include/mcuboot_config
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
