# Add set(CONFIG_USE_middleware_mcu-boot_mem_semc_nor true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mcu-boot_drv_semc_nor)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/semc_nor_memory.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
)

else()

message(SEND_ERROR "middleware_mcu-boot_mem_semc_nor dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
