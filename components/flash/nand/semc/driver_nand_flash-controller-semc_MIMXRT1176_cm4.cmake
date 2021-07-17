include_guard(GLOBAL)
message("driver_nand_flash-controller-semc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_semc_nand_flash.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_nand_flash-common_MIMXRT1176_cm4)

include(driver_semc_MIMXRT1176_cm4)

