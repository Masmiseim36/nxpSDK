include_guard(GLOBAL)
message("middleware_mcu-boot_loadersources component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_flashloader.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_exception_handler.c
    ${CMAKE_CURRENT_LIST_DIR}/src/utilities/src/fsl_assert.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
    ${CMAKE_CURRENT_LIST_DIR}/src/include
    ${CMAKE_CURRENT_LIST_DIR}/src/utilities
)


