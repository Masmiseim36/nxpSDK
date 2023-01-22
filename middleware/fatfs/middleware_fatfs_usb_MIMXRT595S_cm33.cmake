include_guard()
message("middleware_fatfs_usb component is included.")

if(CONFIG_USE_middleware_baremetal_MIMXRT595S_cm33)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk/fsl_usb_disk_bm.c
)
elseif(CONFIG_USE_middleware_freertos-kernel_MIMXRT595S_cm33)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk/fsl_usb_disk_freertos.c
)
else()
    message(WARNING "please config middleware.baremetal_MIMXRT595S_cm33 or middleware.freertos-kernel_MIMXRT595S_cm33 first.")
endif()


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_usb_disk
)


include(middleware_fatfs_MIMXRT595S_cm33)

include(middleware_usb_host_stack_MIMXRT595S_cm33)

include(middleware_usb_host_msd_MIMXRT595S_cm33)

