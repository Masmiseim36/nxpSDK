if(NOT DRIVER_ILI9341_MK66F18_INCLUDED)
    
    set(DRIVER_ILI9341_MK66F18_INCLUDED true CACHE BOOL "driver_ili9341 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ili9341.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK66F18)

endif()
