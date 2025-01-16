# Add set(CONFIG_USE_middleware_tfm_ns_os_wrapper_baremetal true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/os_wrapper/tfm_ns_interface_bare_metal.c
        )

  

