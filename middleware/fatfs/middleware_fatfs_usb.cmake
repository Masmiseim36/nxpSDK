# Add set(CONFIG_USE_middleware_fatfs_usb true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fatfs AND CONFIG_USE_middleware_fatfs_template_usb AND CONFIG_USE_middleware_usb_host_stack AND CONFIG_USE_middleware_usb_host_msd)

if(CONFIG_USE_middleware_baremetal)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk/fsl_usb_disk_bm.c
  )
endif()

if(CONFIG_USE_middleware_freertos-kernel)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk/fsl_usb_disk_freertos.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk
)

else()

message(SEND_ERROR "middleware_fatfs_usb dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
