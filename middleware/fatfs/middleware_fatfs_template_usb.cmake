# Add set(CONFIG_USE_middleware_fatfs_template_usb true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fatfs_usb)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/usb/ffconf.h ${CMAKE_CURRENT_LIST_DIR}/template/usb middleware_fatfs_template_usb)

else()

message(SEND_ERROR "middleware_fatfs_template_usb dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
