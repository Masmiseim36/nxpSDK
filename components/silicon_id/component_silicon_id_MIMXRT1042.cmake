include_guard()
message("component_silicon_id component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_silicon_id.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_driver_common_MIMXRT1042)
     include(driver_common_MIMXRT1042)
endif()
if(NOT (CONFIG_USE_driver_common_MIMXRT1042))
    message(WARNING "Since driver_common_MIMXRT1042 is not included at first or config in config.cmake file, use driver_common_MIMXRT1042/component_silicon_id_imx_device_support_MIMXRT1042 by default.")
    include(driver_common_MIMXRT1042)
    include(component_silicon_id_imx_device_support_MIMXRT1042)
endif()

