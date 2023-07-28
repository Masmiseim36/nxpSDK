# Add set(CONFIG_USE_middleware_mbedtls3x_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/port/mcux_mbedtls_config.h ${CMAKE_CURRENT_LIST_DIR}/port middleware_mbedtls3x_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/port/mcux_mbedtls_accelerator_config.h ${CMAKE_CURRENT_LIST_DIR}/port middleware_mbedtls3x_template)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DMBEDTLS_CONFIG_FILE="mcux_mbedtls_config.h"
  )

endif()

