# Add set(CONFIG_USE_driver_rtt_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/SEGGER_RTT_Conf.h ${CMAKE_CURRENT_LIST_DIR}/template driver_rtt_template.MIMXRT1062)

