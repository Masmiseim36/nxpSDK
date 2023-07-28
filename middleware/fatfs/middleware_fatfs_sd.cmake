# Add set(CONFIG_USE_middleware_fatfs_sd true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fatfs AND CONFIG_USE_middleware_fatfs_template_sd AND CONFIG_USE_middleware_sdmmc_sd)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/fsl_sd_disk/fsl_sd_disk.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/fsl_sd_disk
)

else()

message(SEND_ERROR "middleware_fatfs_sd dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
