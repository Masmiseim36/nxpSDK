# Add set(CONFIG_USE_middleware_usb_pd_config_header true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/output/npw/pd_config/usb_pd_config.h ${CMAKE_CURRENT_LIST_DIR}/output/npw/pd_config middleware_usb_pd_config_header)

