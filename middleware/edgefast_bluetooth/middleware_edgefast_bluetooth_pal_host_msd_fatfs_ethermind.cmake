# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_fatfs_usb)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform/host_msd_fatfs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
