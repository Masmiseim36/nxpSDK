if(NOT DRIVER_WDOG01_MIMXRT1062_INCLUDED)
    
    set(DRIVER_WDOG01_MIMXRT1062_INCLUDED true CACHE BOOL "driver_wdog01 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wdog.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

endif()
