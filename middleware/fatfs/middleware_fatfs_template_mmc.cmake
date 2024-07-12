# Add set(CONFIG_USE_middleware_fatfs_template_mmc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fatfs_mmc)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/mmc/ffconf.h ${CMAKE_CURRENT_LIST_DIR}/template/mmc middleware_fatfs_template_mmc)

else()

message(SEND_ERROR "middleware_fatfs_template_mmc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
