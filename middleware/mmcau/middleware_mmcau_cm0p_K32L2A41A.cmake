if(NOT MIDDLEWARE_MMCAU_CM0P_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_MMCAU_CM0P_K32L2A41A_INCLUDED true CACHE BOOL "middleware_mmcau_cm0p component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(middleware_mmcau_common_files_K32L2A41A)

    include(driver_clock_K32L2A41A)

    include(driver_common_K32L2A41A)

endif()
