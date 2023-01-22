include_guard()
message("component_mflash_rt1050 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mflash_drv.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(component_mflash_common_MIMXRT1052)

include(driver_flexspi_MIMXRT1052)

include(driver_cache_armv7_m7_MIMXRT1052)

