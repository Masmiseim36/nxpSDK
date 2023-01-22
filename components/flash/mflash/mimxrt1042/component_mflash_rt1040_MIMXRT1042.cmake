include_guard()
message("component_mflash_rt1040 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mflash_drv.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(component_mflash_common_MIMXRT1042)

include(driver_flexspi_MIMXRT1042)

include(driver_cache_armv7_m7_MIMXRT1042)

