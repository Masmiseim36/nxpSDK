# Add set(CONFIG_USE_middleware_fatfs true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_USE_middleware_fatfs_template_ram AND CONFIG_USE_middleware_fatfs_ram) OR (CONFIG_USE_middleware_fatfs_template_sd AND CONFIG_USE_middleware_fatfs_sd) OR (CONFIG_USE_middleware_fatfs_template_mmc AND CONFIG_USE_middleware_fatfs_mmc) OR (CONFIG_USE_middleware_fatfs_template_usb AND CONFIG_USE_middleware_fatfs_usb) OR (CONFIG_USE_middleware_fatfs_template_nand AND CONFIG_USE_middleware_fatfs_nand))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/diskio.c
  ${CMAKE_CURRENT_LIST_DIR}/source/ff.c
  ${CMAKE_CURRENT_LIST_DIR}/source/ffsystem.c
  ${CMAKE_CURRENT_LIST_DIR}/source/ffunicode.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source
)

else()

message(SEND_ERROR "middleware_fatfs dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
