if(NOT DRIVER_WDOG01_MIMX8MM6_INCLUDED)
    
    set(DRIVER_WDOG01_MIMX8MM6_INCLUDED true CACHE BOOL "driver_wdog01 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wdog.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MM6)

endif()
