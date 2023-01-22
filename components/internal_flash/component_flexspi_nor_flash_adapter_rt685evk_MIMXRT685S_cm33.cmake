include_guard()
message("component_flexspi_nor_flash_adapter_rt685evk component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/octal_flash/fsl_adapter_flexspi_nor_flash.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_common_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

