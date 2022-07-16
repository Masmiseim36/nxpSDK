include_guard()
message("component_mflash_dummy component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mflash_dummy.c
)


include(component_mflash_file_MIMXRT1052)

