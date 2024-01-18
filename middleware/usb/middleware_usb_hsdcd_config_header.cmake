# Add set(CONFIG_USE_middleware_usb_hsdcd_config_header true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/output/npw/dcd_config/hsdcd/usb_hsdcd_config.h ${CMAKE_CURRENT_LIST_DIR}/output/npw/dcd_config/hsdcd middleware_usb_hsdcd_config_header)

