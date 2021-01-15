if(NOT DRIVER_KPP_MIMXRT1062_INCLUDED)
    
    set(DRIVER_KPP_MIMXRT1062_INCLUDED true CACHE BOOL "driver_kpp component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_kpp.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

endif()
