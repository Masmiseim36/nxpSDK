# Add set(CONFIG_USE_device_MIMXRT1189_mcux_scripts true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1189_system)

else()

message(SEND_ERROR "device_MIMXRT1189_mcux_scripts.MIMXRT1189 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
