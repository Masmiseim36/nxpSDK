# Add set(CONFIG_USE_middleware_edgefast_bluetooth_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_config_template AND ((CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040 AND (CONFIG_BOARD STREQUAL evkmimxrt1040)) OR (CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060 AND (CONFIG_BOARD STREQUAL evkcmimxrt1060)) OR (CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_mimxrt685audevk AND (CONFIG_BOARD STREQUAL mimxrt685audevk)) OR (CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt595 AND (CONFIG_BOARD STREQUAL evkmimxrt595)) OR (CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_evkbmimxrt1170 AND (CONFIG_BOARD STREQUAL evkbmimxrt1170))))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/output/templates/config/mbedtls/mbedtls_config_client.h ${CMAKE_CURRENT_LIST_DIR}/output/templates/config/mbedtls middleware_edgefast_bluetooth_template)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
