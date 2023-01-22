include_guard()
message("middleware_fatfs component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/diskio.c
    ${CMAKE_CURRENT_LIST_DIR}/source/ff.c
    ${CMAKE_CURRENT_LIST_DIR}/source/ffsystem.c
    ${CMAKE_CURRENT_LIST_DIR}/source/ffunicode.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source
)


#OR Logic component
if(CONFIG_USE_middleware_fatfs_template_ram_MIMXRT1052)
     include(middleware_fatfs_template_ram_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_ram_MIMXRT1052)
     include(middleware_fatfs_ram_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_template_sd_MIMXRT1052)
     include(middleware_fatfs_template_sd_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_sd_MIMXRT1052)
     include(middleware_fatfs_sd_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_template_sdspi_MIMXRT1052)
     include(middleware_fatfs_template_sdspi_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_sdspi_MIMXRT1052)
     include(middleware_fatfs_sdspi_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_template_mmc_MIMXRT1052)
     include(middleware_fatfs_template_mmc_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_mmc_MIMXRT1052)
     include(middleware_fatfs_mmc_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_template_usb_MIMXRT1052)
     include(middleware_fatfs_template_usb_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_usb_MIMXRT1052)
     include(middleware_fatfs_usb_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_template_nand_MIMXRT1052)
     include(middleware_fatfs_template_nand_MIMXRT1052)
endif()
if(CONFIG_USE_middleware_fatfs_nand_MIMXRT1052)
     include(middleware_fatfs_nand_MIMXRT1052)
endif()
if(NOT (CONFIG_USE_middleware_fatfs_template_ram_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_ram_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_template_sd_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_sd_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_template_sdspi_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_sdspi_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_template_mmc_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_mmc_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_template_usb_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_usb_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_template_nand_MIMXRT1052 OR CONFIG_USE_middleware_fatfs_nand_MIMXRT1052))
    message(WARNING "Since middleware_fatfs_template_ram_MIMXRT1052/middleware_fatfs_ram_MIMXRT1052/middleware_fatfs_template_sd_MIMXRT1052/middleware_fatfs_sd_MIMXRT1052/middleware_fatfs_template_sdspi_MIMXRT1052/middleware_fatfs_sdspi_MIMXRT1052/middleware_fatfs_template_mmc_MIMXRT1052/middleware_fatfs_mmc_MIMXRT1052/middleware_fatfs_template_usb_MIMXRT1052/middleware_fatfs_usb_MIMXRT1052/middleware_fatfs_template_nand_MIMXRT1052/middleware_fatfs_nand_MIMXRT1052 is not included at first or config in config.cmake file, use middleware_fatfs_template_ram_MIMXRT1052/middleware_fatfs_ram_MIMXRT1052 by default.")
    include(middleware_fatfs_template_ram_MIMXRT1052)
    include(middleware_fatfs_ram_MIMXRT1052)
endif()

