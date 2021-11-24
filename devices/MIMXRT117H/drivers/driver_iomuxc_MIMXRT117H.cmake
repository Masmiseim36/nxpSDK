if(NOT DRIVER_IOMUXC_MIMXRT117H_INCLUDED)
    
    set(DRIVER_IOMUXC_MIMXRT117H_INCLUDED true CACHE BOOL "driver_iomuxc component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_MIMXRT117H)

endif()
