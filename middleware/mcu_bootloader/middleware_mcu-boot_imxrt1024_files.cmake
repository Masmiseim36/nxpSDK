# Add set(CONFIG_USE_middleware_mcu-boot_imxrt1024_files true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1024xxxxx))

else()

message(SEND_ERROR "middleware_mcu-boot_imxrt1024_files dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
