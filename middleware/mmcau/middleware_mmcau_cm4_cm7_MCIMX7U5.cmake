if(NOT MIDDLEWARE_MMCAU_CM4_CM7_MCIMX7U5_INCLUDED)
    
    set(MIDDLEWARE_MMCAU_CM4_CM7_MCIMX7U5_INCLUDED true CACHE BOOL "middleware_mmcau_cm4_cm7 component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(middleware_mmcau_common_files_MCIMX7U5)

    include(driver_clock_MCIMX7U5)

    include(driver_common_MCIMX7U5)

endif()
