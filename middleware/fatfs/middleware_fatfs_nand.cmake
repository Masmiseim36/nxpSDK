# Add set(CONFIG_USE_middleware_fatfs_nand true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fatfs AND CONFIG_USE_middleware_dhara AND CONFIG_USE_middleware_fatfs_template_nand AND (CONFIG_USE_driver_nand_flash-controller-semc OR CONFIG_USE_driver_nand_flash-controller-flexspi))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/fsl_nand_disk/fsl_nand_disk.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/fsl_nand_disk
)

else()

message(SEND_ERROR "middleware_fatfs_nand dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
