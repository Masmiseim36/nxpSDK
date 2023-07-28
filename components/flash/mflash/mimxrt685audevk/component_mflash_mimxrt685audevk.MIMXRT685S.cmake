# Add set(CONFIG_USE_component_mflash_mimxrt685audevk true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_mflash_common AND (CONFIG_BOARD STREQUAL mimxrt685audevk) AND CONFIG_USE_driver_flexspi AND CONFIG_USE_driver_cache_cache64)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mflash_drv.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DMFLASH_FILE_BASEADDR=7340032
  )

endif()

else()

message(SEND_ERROR "component_mflash_mimxrt685audevk.MIMXRT685S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
