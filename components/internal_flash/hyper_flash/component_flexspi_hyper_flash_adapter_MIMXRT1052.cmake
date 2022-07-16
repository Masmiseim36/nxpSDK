include_guard()
message("component_flexspi_hyper_flash_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_flexspi_hyper_nor_flash.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_common_MIMXRT1052)

include(driver_flexspi_MIMXRT1052)

include(component_flexspi_nor_flash_adapter_MIMXRT1052)

include(component_flexspi_hyper_flash_adapter_config_rt1050_MIMXRT1052)

