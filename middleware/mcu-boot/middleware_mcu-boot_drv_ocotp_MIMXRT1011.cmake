include_guard(GLOBAL)
message("middleware_mcu-boot_drv_ocotp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ocotp/bl_ocotp.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ocotp
)


