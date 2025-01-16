# Add set(CONFIG_USE_component_psa_crypto_driver_els_pkc_oracle_rw61x true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/els_pkc/oracle/platforms/rw61x
        )

  
