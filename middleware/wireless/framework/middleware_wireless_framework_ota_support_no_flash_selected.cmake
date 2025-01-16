# Add set(CONFIG_USE_middleware_wireless_framework_ota_support_no_flash_selected true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/services/OtaSupport/Source/OtaInternalFlash.c
          ${CMAKE_CURRENT_LIST_DIR}/services/OtaSupport/Source/OtaExternalFlash.c
        )

  

