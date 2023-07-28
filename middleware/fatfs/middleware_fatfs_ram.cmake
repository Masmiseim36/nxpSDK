# Add set(CONFIG_USE_middleware_fatfs_ram true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fatfs AND CONFIG_USE_middleware_fatfs_template_ram AND CONFIG_USE_driver_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/fsl_ram_disk/fsl_ram_disk.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/fsl_ram_disk
)

else()

message(SEND_ERROR "middleware_fatfs_ram dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
