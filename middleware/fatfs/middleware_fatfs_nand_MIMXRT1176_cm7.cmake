include_guard(GLOBAL)
message("middleware_fatfs_nand component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_nand_disk/fsl_nand_disk.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/fsl_nand_disk
)


#OR Logic component
if(CONFIG_USE_driver_nand_flash-controller-semc_MIMXRT1176_cm7)
     include(driver_nand_flash-controller-semc_MIMXRT1176_cm7)
endif()
if(CONFIG_USE_driver_nand_flash-controller-flexspi_MIMXRT1176_cm7)
     include(driver_nand_flash-controller-flexspi_MIMXRT1176_cm7)
endif()
if(NOT (CONFIG_USE_driver_nand_flash-controller-semc_MIMXRT1176_cm7 OR CONFIG_USE_driver_nand_flash-controller-flexspi_MIMXRT1176_cm7))
    message(WARNING "Since driver_nand_flash-controller-semc_MIMXRT1176_cm7/driver_nand_flash-controller-flexspi_MIMXRT1176_cm7 is not included at first or config in config.cmake file, use driver_nand_flash-controller-semc_MIMXRT1176_cm7 by default.")
    include(driver_nand_flash-controller-semc_MIMXRT1176_cm7)
endif()

include(middleware_fatfs_MIMXRT1176_cm7)

