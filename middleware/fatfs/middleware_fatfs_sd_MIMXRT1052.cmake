include_guard()
message("middleware_fatfs_sd component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_sd_disk/fsl_sd_disk.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_sd_disk
)


include(middleware_fatfs_MIMXRT1052)

include(middleware_sdmmc_sd_MIMXRT1052)

