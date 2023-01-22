include_guard()
message("middleware_fatfs_mmc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_mmc_disk/fsl_mmc_disk.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_mmc_disk
)


include(middleware_fatfs_MIMXRT595S_cm33)

include(middleware_sdmmc_mmc_MIMXRT595S_cm33)

