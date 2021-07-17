include_guard(GLOBAL)
message("component_mflash_rt1170 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mflash_drv.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_driver_cache_armv7_m7_MIMXRT1176_cm7)
     include(driver_cache_armv7_m7_MIMXRT1176_cm7)
endif()
if(CONFIG_USE_driver_cache_lmem_MIMXRT1176_cm7)
     include(driver_cache_lmem_MIMXRT1176_cm7)
endif()
if(NOT (CONFIG_USE_driver_cache_armv7_m7_MIMXRT1176_cm7 OR CONFIG_USE_driver_cache_lmem_MIMXRT1176_cm7))
    message(WARNING "Since driver_cache_armv7_m7_MIMXRT1176_cm7/driver_cache_lmem_MIMXRT1176_cm7 is not included at first or config in config.cmake file, use driver_cache_armv7_m7_MIMXRT1176_cm7 by default.")
    include(driver_cache_armv7_m7_MIMXRT1176_cm7)
endif()

include(component_mflash_common_MIMXRT1176_cm7)

include(driver_flexspi_MIMXRT1176_cm7)

