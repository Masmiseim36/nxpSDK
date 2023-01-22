include_guard()
message("middleware_fatfs_ram component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_ram_disk/fsl_ram_disk.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_ram_disk
)


include(middleware_fatfs_MIMXRT1052)

include(driver_common_MIMXRT1052)

